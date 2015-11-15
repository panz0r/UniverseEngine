#include "resource_manager.h"
#include "render_device.h"

#include <renderer_d3d12/renderer/render_atom.h>
#include <d3dcompiler.h>

#include <math/hash.h>

namespace ue
{


static const D3D12_RESOURCE_DIMENSION dimension_lut[] = 
{
	D3D12_RESOURCE_DIMENSION_UNKNOWN,
	D3D12_RESOURCE_DIMENSION_BUFFER,
	D3D12_RESOURCE_DIMENSION_TEXTURE1D,
	D3D12_RESOURCE_DIMENSION_TEXTURE2D,
	D3D12_RESOURCE_DIMENSION_TEXTURE3D,
};

static const D3D12_SRV_DIMENSION srv_dimension_lut[] = 
{
	D3D12_SRV_DIMENSION_UNKNOWN,
	D3D12_SRV_DIMENSION_BUFFER,
	D3D12_SRV_DIMENSION_TEXTURE1D,
	D3D12_SRV_DIMENSION_TEXTURE1DARRAY,
	D3D12_SRV_DIMENSION_TEXTURE2D,
	D3D12_SRV_DIMENSION_TEXTURE2DARRAY,
	D3D12_SRV_DIMENSION_TEXTURE2DMS,
	D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY,
	D3D12_SRV_DIMENSION_TEXTURE3D,
	D3D12_SRV_DIMENSION_TEXTURECUBE,
	D3D12_SRV_DIMENSION_TEXTURECUBEARRAY
};

static const DXGI_FORMAT format_lut[] =
{
	DXGI_FORMAT_R8G8B8A8_UNORM,
};

ResourceManager::ResourceManager(D3D12RenderDevice & render_device)
	: _render_device(render_device)
	, _srv_uav_cbv_offline_heap(*render_device.device(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 20)
	, _sampler_offline_heap(*render_device.device(), D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, 10)
	, _rtv_heap(*render_device.device(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 10)
	, _dsv_heap(*render_device.device(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1)
{
	ID3D12Device * device = render_device.device();

	// Create resource heap
	{
		D3D12_HEAP_DESC desc = {};
		desc.Alignment = 0;
		desc.Flags = D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES;
		desc.Properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		desc.SizeInBytes = 1024 * 1024 * 128;
		HRESULT hr = device->CreateHeap(&desc, IID_PPV_ARGS(&_resource_heap));
		UENSURE(SUCCEEDED(hr));
		_resource_allocator.initialize(nullptr, 1024 * 1024 * 128, 65536);
	}

	// Create upload heap
	{
		HRESULT hr = device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(128 * 1024 * 1024),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&_upload_heap));
		UENSURE(SUCCEEDED(hr));
		_upload_heap_offset = 0;
	}

	{
		// Create basic root signature
		CD3DX12_DESCRIPTOR_RANGE ranges[4];
		CD3DX12_ROOT_PARAMETER parameters[3];
		ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 5, 0);
		ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);
		ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 5, 0);
		ranges[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);

		parameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
		parameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);
		parameters[2].InitAsDescriptorTable(1, &ranges[2], D3D12_SHADER_VISIBILITY_ALL);

		D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		CD3DX12_ROOT_SIGNATURE_DESC root_signature_desc;
		root_signature_desc.Init(_countof(parameters), parameters, 0, nullptr, flags);

		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		D3D12SerializeRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
		device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&_root_signature));
	}

}

ResourceManager::~ResourceManager()
{
}

RenderResource* ResourceManager::lookup_resource(RenderHandle resource_handle)
{
	switch (resource_handle._type)
	{
	case RenderResource::Texture:
		return _textures.get_ptr<TextureResource>(resource_handle);
	case RenderResource::Buffer:
		return _buffers.get_ptr<BufferResource>(resource_handle);
	case RenderResource::PipelineState:
		return _pipeline_states.get_ptr<PipelineStateResource>(resource_handle);
	case RenderResource::InstancedRenderAtom:
		return _render_atoms.get_ptr<RenderAtom>(resource_handle);
	case RenderResource::RenderTarget:
		return _render_targets.get_ptr<RenderTargetResource>(resource_handle);
	default:
		UASSERT(false, "Unknown resource type");
		break;
	}
	return nullptr;
}

RenderHandle ResourceManager::create_texture(ID3D12GraphicsCommandList *command_list, const TextureDesc &desc, const void *data)
{
	size_t required_size = sizeof(TextureResource);

	unsigned handle_count = desc.dynamic ? _render_device.buffer_count() : 1;
	required_size += sizeof(D3D12_CPU_DESCRIPTOR_HANDLE) * handle_count;
	required_size += sizeof(DescriptorHeapHandle) * handle_count;
	// Todo, UAV access

	// Alloc enough memory to hold all required data
	TextureResource *texture = new (malloc(required_size)) TextureResource();

	// fixup pointers
	char *ptr = ((char*)texture) + sizeof(TextureResource);
	texture->handles = (DescriptorHeapHandle*)ptr;
	ptr += sizeof(DescriptorHeapHandle) * handle_count;
	texture->srv = (D3D12_CPU_DESCRIPTOR_HANDLE*)ptr;
	ptr += sizeof(D3D12_CPU_DESCRIPTOR_HANDLE) * handle_count;


	D3D12_RESOURCE_DESC resource_desc = {};
	resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resource_desc.Dimension = dimension_lut[desc.dimension];
	resource_desc.Width = desc.width;
	resource_desc.Height = desc.height;
	resource_desc.DepthOrArraySize = desc.depth;
	resource_desc.MipLevels = desc.mip_levels;
	resource_desc.Format = format_lut[desc.format];
	resource_desc.SampleDesc.Count = 1;
	resource_desc.SampleDesc.Quality = 0;

	ID3D12Device *device = _render_device.device();

	// Alloc gpu memory
	unsigned required_resource_size = desc.width * desc.height * sizeof(unsigned); // Todo: actual pixel size

	PlacedGPUResource placed_resource(&_resource_allocator, required_resource_size);

	HRESULT hr = device->CreatePlacedResource(_resource_heap.Get(),
		placed_resource.offset(),
		&resource_desc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(placed_resource.init_resource()));
	UENSURE(SUCCEEDED(hr));

	// validate memory footprint
	UENSURE(GetRequiredIntermediateSize(placed_resource.resource(), 0, 1) == required_resource_size);

	// Setup sub resource data
	D3D12_SUBRESOURCE_DATA sub_resource_data = {};
	sub_resource_data.pData = data;
	sub_resource_data.RowPitch = desc.width * sizeof(unsigned); // TODO: use real pixel size
	sub_resource_data.SlicePitch = desc.height * sub_resource_data.RowPitch;

	// Make sure upload heap offset is properly aligned for texture upload
	_upload_heap_offset = ALIGN(_upload_heap_offset, D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT);

	// commit upload command to command list and set initial resource barrier accordingly
	UpdateSubresources(command_list, placed_resource.resource(), _upload_heap.Get(), _upload_heap_offset, 0, 1, &sub_resource_data);
	command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(placed_resource.resource(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	_upload_heap_offset += required_resource_size;

	// Setup shader resource view
	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	srv_desc.ViewDimension = srv_dimension_lut[4]; // tdodo
	srv_desc.Format = format_lut[desc.format];
	srv_desc.Texture2D.MipLevels = desc.mip_levels;
	srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	// aquire new handle from offline descriptor heap
	DescriptorHeapHandle descriptor_heap_handle = _srv_uav_cbv_offline_heap.aquire_handle();
	memcpy(texture->handles, &descriptor_heap_handle, sizeof(DescriptorHeapHandle));

	// create cpu descriptor handle for resource
	D3D12_CPU_DESCRIPTOR_HANDLE srv_handle = _srv_uav_cbv_offline_heap.cpu_descriptor_handle(descriptor_heap_handle);
	device->CreateShaderResourceView(texture->placed_resource.resource(), &srv_desc, srv_handle);

	// copy handle data to resource part
	memcpy(texture->srv, &srv_handle, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));

	Handle texture_handle = _textures.insert(texture, RenderResource::Texture);
	return texture_handle;
}

void ResourceManager::destroy_texture(RenderHandle handle)
{
	TextureResource *texture = _textures.get_ptr<TextureResource>(handle);
	_srv_uav_cbv_offline_heap.release_handle(*texture->handles);
	_textures.remove(handle);
	
	texture->placed_resource.deallocate();
	texture->~TextureResource();

	free(texture);
}

RenderHandle ResourceManager::create_render_target_from_resource(ID3D12Resource *resource)
{
	size_t required_size = sizeof(RenderTargetResource);

	required_size += sizeof(D3D12_CPU_DESCRIPTOR_HANDLE);
	required_size += sizeof(DescriptorHeapHandle);

	// Alloc enough memory to hold all required data
	RenderTargetResource *rt = new (malloc(required_size)) RenderTargetResource();
	memset(rt, 0, required_size);
	rt->type = RenderResource::RenderTarget;

	// fixup pointers
	char *ptr = ((char*)rt) + sizeof(RenderTargetResource);
	rt->handle = (DescriptorHeapHandle*)ptr;
	ptr += sizeof(DescriptorHeapHandle);
	rt->rtv = (D3D12_CPU_DESCRIPTOR_HANDLE*)ptr;
	ptr += sizeof(D3D12_CPU_DESCRIPTOR_HANDLE);


	// aquire new handle from offline descriptor heap
	DescriptorHeapHandle descriptor_heap_handle = _rtv_heap.aquire_handle();
	memcpy(rt->handle, &descriptor_heap_handle, sizeof(DescriptorHeapHandle));

	ID3D12Device *device = _render_device.device();
	rt->resource = ComPtr<ID3D12Resource>(resource);

	// create cpu descriptor handle for resource
	D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = _rtv_heap.cpu_descriptor_handle(descriptor_heap_handle);
	device->CreateRenderTargetView(rt->resource.Get(), nullptr, rtv_handle);

	// copy handle data to resource part
	memcpy(rt->rtv, &rtv_handle, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
	rt->current_state = RenderTargetResource::PRESENT;

	RenderHandle rt_handle = _render_targets.insert(rt, RenderResource::RenderTarget);
	return rt_handle;
}


RenderHandle ResourceManager::create_vertex_buffer(const BufferDesc &desc, const void *data, RenderResourceContext &rrc)
{
	ID3D12Device *device = _render_device.device();

	size_t required_size = sizeof(BufferResource);
	unsigned handle_count = desc.dynamic ? _render_device.buffer_count() : 1;
	required_size += sizeof(D3D12_VERTEX_BUFFER_VIEW) * handle_count;
	// Alloc enough memory to hold all required data
	BufferResource *buffer = new (malloc(required_size)) BufferResource();
	buffer->type = RenderResource::Buffer;

	// fixup pointers
	char *ptr = ((char*)buffer) + sizeof(BufferResource);
	//buffer->handles = (DescriptorHeapHandle*)ptr;
	//ptr += sizeof(DescriptorHeapHandle) * handle_count;
	buffer->srv = nullptr; //
	buffer->uav = nullptr;
	buffer->vbv = (D3D12_VERTEX_BUFFER_VIEW*)ptr;
	

	// Alloc gpu memory
	unsigned required_resource_size = desc.size;
	PlacedGPUResource placed_resource(&_resource_allocator, required_resource_size);

	HRESULT hr = device->CreatePlacedResource(_resource_heap.Get(),
		placed_resource.offset(),
		&CD3DX12_RESOURCE_DESC::Buffer(desc.size),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(placed_resource.init_resource()));
	UENSURE(SUCCEEDED(hr));
	// validate memory footprint
	UENSURE(GetRequiredIntermediateSize(placed_resource.resource(), 0, 1) == required_resource_size);

	// Setup sub resource data
	D3D12_SUBRESOURCE_DATA sub_resource_data = {};
	sub_resource_data.pData = data;
	sub_resource_data.RowPitch = desc.size;
		
	rrc.add_resource(placed_resource.resource(), sub_resource_data, required_resource_size, 256, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

	// Setup vertexbuffer view
	D3D12_VERTEX_BUFFER_VIEW vbv = {};
	vbv.BufferLocation = placed_resource.resource()->GetGPUVirtualAddress();
	vbv.SizeInBytes = desc.size;
	vbv.StrideInBytes = desc.stride;
		
	// copy vbv to resource
	memcpy(buffer->vbv, &vbv, sizeof(D3D12_VERTEX_BUFFER_VIEW));
	buffer->placed_resource = placed_resource;

	RenderHandle buffer_handle = _buffers.insert(buffer, RenderResource::Buffer);
	return buffer_handle;
}

RenderHandle ResourceManager::create_index_buffer(const BufferDesc &desc, const void *data, RenderResourceContext &rrc)
{
	ID3D12Device *device = _render_device.device();

	size_t required_size = sizeof(BufferResource);
	unsigned handle_count = desc.dynamic ? _render_device.buffer_count() : 1;
	required_size += sizeof(D3D12_INDEX_BUFFER_VIEW) * handle_count;
	// Alloc enough memory to hold all required data
	BufferResource *buffer = new (malloc(required_size)) BufferResource();
	buffer->type = RenderResource::Buffer;

	// fixup pointers
	char *ptr = ((char*)buffer) + sizeof(BufferResource);
	buffer->srv = nullptr; 
	buffer->uav = nullptr;
	buffer->ibv = (D3D12_INDEX_BUFFER_VIEW*)ptr;


	// Alloc gpu memory
	unsigned required_resource_size = desc.size;
	PlacedGPUResource placed_resource(&_resource_allocator, required_resource_size);

	HRESULT hr = device->CreatePlacedResource(_resource_heap.Get(),
		placed_resource.offset(),
		&CD3DX12_RESOURCE_DESC::Buffer(desc.size),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(placed_resource.init_resource()));
	UENSURE(SUCCEEDED(hr));
	// validate memory footprint
	UENSURE(GetRequiredIntermediateSize(placed_resource.resource(), 0, 1) == required_resource_size);

	// Setup sub resource data
	D3D12_SUBRESOURCE_DATA sub_resource_data = {};
	sub_resource_data.pData = data;
	sub_resource_data.RowPitch = desc.size;

	rrc.add_resource(placed_resource.resource(), sub_resource_data, required_resource_size, 256, D3D12_RESOURCE_STATE_INDEX_BUFFER);

	// Setup vertexbuffer view
	D3D12_INDEX_BUFFER_VIEW ibv = {};
	ibv.BufferLocation = placed_resource.resource()->GetGPUVirtualAddress();
	ibv.SizeInBytes = desc.size;
	ibv.Format = (DXGI_FORMAT)desc.format;

	// copy vbv to resource
	memcpy(buffer->ibv, &ibv, sizeof(D3D12_INDEX_BUFFER_VIEW));
	buffer->placed_resource = placed_resource;

	RenderHandle buffer_handle = _buffers.insert(buffer, RenderResource::Buffer);
	return buffer_handle;
}

RenderHandle ResourceManager::create_constant_buffer(const BufferDesc &desc, const void *data, RenderResourceContext &rrc)
{
	ID3D12Device *device = _render_device.device();

	size_t required_size = sizeof(BufferResource);
	unsigned handle_count = desc.dynamic ? _render_device.buffer_count() : 1;
	required_size += sizeof(D3D12_CPU_DESCRIPTOR_HANDLE) * handle_count;
	required_size += sizeof(DescriptorHeapHandle) * handle_count;
	// Alloc enough memory to hold all required data
	BufferResource *buffer = new (malloc(required_size)) BufferResource();
	memset(buffer, 0, required_size);

	buffer->type = RenderResource::Buffer;

	// fixup pointers
	char *ptr = ((char*)buffer) + sizeof(BufferResource);
	buffer->handles = (DescriptorHeapHandle*)ptr;
	ptr += sizeof(DescriptorHeapHandle);
	buffer->cbv = (D3D12_CPU_DESCRIPTOR_HANDLE*)ptr;
	buffer->uav = nullptr;

	// Alloc gpu memory
	unsigned required_resource_size = ALIGN(desc.size, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
	PlacedGPUResource placed_resource(&_resource_allocator, required_resource_size);

	HRESULT hr = device->CreatePlacedResource(_resource_heap.Get(),
		placed_resource.offset(),
		&CD3DX12_RESOURCE_DESC::Buffer(required_resource_size),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(placed_resource.init_resource()));
	UENSURE(SUCCEEDED(hr));
	// validate memory footprint
	UENSURE(GetRequiredIntermediateSize(placed_resource.resource(), 0, 1) == required_resource_size);

	// Setup sub resource data
	D3D12_SUBRESOURCE_DATA sub_resource_data = {};
	sub_resource_data.pData = data;
	sub_resource_data.RowPitch = required_resource_size;

	rrc.add_resource(placed_resource.resource(), sub_resource_data, required_resource_size, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

	// Setup shader resource view
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc = {};
	cbv_desc.BufferLocation = placed_resource.resource()->GetGPUVirtualAddress();
	cbv_desc.SizeInBytes = required_resource_size;
	
	// aquire new handle from offline descriptor heap
	DescriptorHeapHandle descriptor_heap_handle = _srv_uav_cbv_offline_heap.aquire_handle();
	memcpy(buffer->handles, &descriptor_heap_handle, sizeof(DescriptorHeapHandle));

	// create cpu descriptor handle for resource
	D3D12_CPU_DESCRIPTOR_HANDLE cbv_handle = _srv_uav_cbv_offline_heap.cpu_descriptor_handle(descriptor_heap_handle);
	device->CreateConstantBufferView(&cbv_desc, cbv_handle);

	// copy handle data to resource part
	memcpy(buffer->cbv, &cbv_handle, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));

	// map dynamic data
	// hr = placed_resource.resource()->Map(0, nullptr, &buffer->mapped_data);

	buffer->placed_resource = placed_resource;

	RenderHandle buffer_handle = _buffers.insert(buffer, RenderResource::Buffer);
	return buffer_handle;

}


RenderHandle ResourceManager::create_render_atom(const InstancedRenderAtomDesc &desc)
{
	size_t required_size = sizeof(InstancedRenderAtom);
	required_size += sizeof(ID3D12PipelineState*);
	required_size += sizeof(D3D12_INDEX_BUFFER_VIEW);
	required_size += sizeof(D3D12_VERTEX_BUFFER_VIEW) * desc.vertex_buffer_count;
	required_size += sizeof(D3D12_CPU_DESCRIPTOR_HANDLE) * (desc.cbv_count + desc.uav_count + desc.srv_count + desc.sampler_count);
	// check size

	InstancedRenderAtom *render_atom = new (malloc(required_size)) InstancedRenderAtom();
	memset(render_atom, 0, required_size);

	render_atom->size = required_size;
	render_atom->type = RenderResource::InstancedRenderAtom;


	render_atom->index_count_per_instance = desc.index_count_per_instance;
	render_atom->instance_count = desc.instance_count;
	render_atom->start_index_location = desc.start_index_location;
	render_atom->base_vertex_location = desc.base_vertex_location;
	render_atom->start_instance_location = desc.start_instance_location;
	render_atom->topology = desc.topology;

	char *p = reinterpret_cast<char*>(render_atom);
	p += sizeof(InstancedRenderAtom);

	auto pso = (PipelineStateResource*)lookup_resource(desc.pso);
	UENSURE(pso);
	ID3D12PipelineState *pipeline_state = pso->_pipeline_state.Get();
	memcpy(p, &pipeline_state, sizeof(ID3D12PipelineState*));
	p += sizeof(ID3D12PipelineState*);
	auto index_buffer = (BufferResource*)lookup_resource(desc.index_buffer);
	UENSURE(index_buffer);
	memcpy(p, index_buffer->ibv, sizeof(D3D12_INDEX_BUFFER_VIEW));
	p += sizeof(D3D12_INDEX_BUFFER_VIEW);
	for (unsigned i = 0; i < desc.vertex_buffer_count; ++i) {
		auto vertex_buffer = (BufferResource*)lookup_resource(desc.vertex_buffers[i]);
		UENSURE(vertex_buffer);
		memcpy(p, vertex_buffer->vbv, sizeof(D3D12_VERTEX_BUFFER_VIEW));
		p += sizeof(D3D12_VERTEX_BUFFER_VIEW);
		render_atom->vertex_buffer_count++;
	}

	for (unsigned i = 0; i < desc.srv_count; ++i) {
		auto srv = lookup_resource(desc.srvs[i]);
		UENSURE(srv);
		memcpy(p, srv, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
		p += sizeof(D3D12_CPU_DESCRIPTOR_HANDLE);
		render_atom->srv_count++;
		render_atom->srv_hash = default_hash(srv, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE), render_atom->srv_hash);
	}

	for (unsigned i = 0; i < desc.uav_count; ++i) {
		auto uav = lookup_resource(desc.srvs[i]);
		UENSURE(uav);
		memcpy(p, uav, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
		p += sizeof(D3D12_CPU_DESCRIPTOR_HANDLE);
		render_atom->uav_count++;
		render_atom->uav_hash = default_hash(uav, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE), render_atom->uav_hash);
	}

	for (unsigned i = 0; i < desc.cbv_count; ++i) {
		auto cbv_resource = (BufferResource*)lookup_resource(desc.cbvs[i]);
		UENSURE(cbv_resource);
		memcpy(p, cbv_resource->cbv, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
		p += sizeof(D3D12_CPU_DESCRIPTOR_HANDLE);
		render_atom->cbv_count++;
		render_atom->cbv_hash = default_hash(cbv_resource->cbv, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE), render_atom->cbv_hash);
	}

	for (unsigned i = 0; i < desc.sampler_count; ++i) {
		auto sampler = lookup_resource(desc.samplers[i]);
		UENSURE(sampler);
		memcpy(p, sampler, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
		p += sizeof(D3D12_CPU_DESCRIPTOR_HANDLE);
		render_atom->sampler_count++;
		render_atom->sampler_hash = default_hash(sampler, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE), render_atom->sampler_hash);
	}

	RenderHandle handle = _render_atoms.insert(render_atom, RenderResource::InstancedRenderAtom);
	return handle;
}

RenderHandle ResourceManager::create_pipeline_state_object(const PipelineStateObjectDesc & desc)
{
	ID3D12Device *device = _render_device.device();

	// Temp code

		ComPtr<ID3DBlob> vertex_shader_blob;
		ComPtr<ID3DBlob> pixel_shader_blob;

		HRESULT hr = D3DReadFileToBlob(L"shaders/VertexShader.cso", &vertex_shader_blob);
		hr = D3DReadFileToBlob(L"shaders/PixelShader.cso", &pixel_shader_blob);

		D3D12_INPUT_ELEMENT_DESC input_element_descs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};

		D3D12_GRAPHICS_PIPELINE_STATE_DESC gps_desc = {};
		gps_desc.InputLayout = { input_element_descs, _countof(input_element_descs) };
		gps_desc.pRootSignature = _root_signature.Get();
		gps_desc.VS = { vertex_shader_blob->GetBufferPointer(), vertex_shader_blob->GetBufferSize() };
		gps_desc.PS = { pixel_shader_blob->GetBufferPointer(), pixel_shader_blob->GetBufferSize() };
		gps_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		gps_desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		gps_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		gps_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		gps_desc.DepthStencilState.DepthEnable = false;
		gps_desc.DepthStencilState.StencilEnable = false;
		gps_desc.SampleMask = 0xffffffff;
		gps_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		gps_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gps_desc.NumRenderTargets = 1;
		gps_desc.SampleDesc.Count = 1;
		gps_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		

		PipelineStateResource *pipeline_state = new PipelineStateResource();
		pipeline_state->type = RenderResource::PipelineState;

		hr = device->CreateGraphicsPipelineState(&gps_desc, IID_PPV_ARGS(&pipeline_state->_pipeline_state));
		RenderHandle handle = _pipeline_states.insert(pipeline_state, RenderResource::PipelineState);

	return handle;
}

}