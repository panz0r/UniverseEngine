#include "resource_manager.h"
#include "d3d12_render_device.h"
#include "render_atom.h"

#include <d3dcompiler.h>
#include <math/hash.h>

namespace ue
{

extern unsigned __cbv_srv_uav_descriptor_increment_size = 0;
extern unsigned __sampler_descriptor_increment_size = 0;


template<typename T>
T* offset_ptr(void *ptr, unsigned index) {
	return reinterpret_cast<T*>(reinterpret_cast<size_t>(ptr) + sizeof(T) * index);
}

template<typename T>
T* resource_new(size_t size) {
	void * mem = malloc(size);
	memset(mem, 0, size);
	T* resource = new (mem) T();
	resource->type = T::Type;
	return resource;
}

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
	DXGI_FORMAT_D32_FLOAT,
};

ResourceManager::ResourceManager(D3D12RenderDevice & render_device)
	: _render_device(render_device)
	, _srv_uav_cbv_offline_heap(*render_device.device(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 20, 1)
	, _frame_srv_uav_cbv_offline_heap(*render_device.device(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, _render_device.back_buffer_count()*20, _render_device.back_buffer_count())
	, _sampler_offline_heap(*render_device.device(), D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, 10, 1)
	, _rtv_heap(*render_device.device(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 10, 1)
	, _dsv_heap(*render_device.device(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, 1)
{
	ID3D12Device * device = render_device.device();

	__cbv_srv_uav_descriptor_increment_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	__sampler_descriptor_increment_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	
	{
		D3D12DescriptorAllocatorDesc desc;
		desc.flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		desc.size = 128;
		_cbv_srv_uav_descriptor_allocator = new D3D12DescriptorAllocator(device, desc);

		desc.type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
		desc.size = 32;
		_sampler_descriptor_allocator = new D3D12DescriptorAllocator(device, desc);

		desc.type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.size = 32;
		_rtv_descriptor_allocator = new D3D12DescriptorAllocator(device, desc);

		desc.type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		desc.size = 16;
		_dsv_descriptor_allocator = new D3D12DescriptorAllocator(device, desc);
	}

	{
		D3D12AllocatorDesc desc;
		desc.size = 1024 * 1024 * 128;
		desc.alignment = 1024 * 64;
		desc.slot_size = 1024 * 64;
		desc.flags = D3D12_HEAP_FLAG_NONE;
		desc.type = D3D12_HEAP_TYPE_DEFAULT;
		_placement_resource_heap_allocator = new D3D12HeapAllocator(device, desc);
	}
	
	{
		D3D12AllocatorDesc desc;
		desc.size = 1024 * 1024 * 128;
		desc.alignment = D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT;
		desc.slot_size = D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT;
		desc.flags = D3D12_HEAP_FLAG_NONE;
		desc.type = D3D12_HEAP_TYPE_UPLOAD;
		_cb_heap_allocator = new D3D12ResourceAllocator(device, desc);
	}

	// Create upload heap
	{
		const uint64_t size = 1024 * 1024 * 128;
		HRESULT hr = device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(size),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&_upload_heap));
		UENSURE(SUCCEEDED(hr));
		_upload_heap_offset = 0;
	}

	{
		// Create basic root signature
		CD3DX12_DESCRIPTOR_RANGE ranges[4];


		ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 5, 0);
		ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);
		ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 3, 1);
		ranges[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);


		CD3DX12_ROOT_PARAMETER parameters[3];
		parameters[0].InitAsConstantBufferView(0);
		parameters[1].InitAsDescriptorTable(1, &ranges[2]);
		//parameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);
		parameters[2].InitAsConstants(4, 4);

//		parameters[2].InitAsDescriptorTable(1, &ranges[2], D3D12_SHADER_VISIBILITY_ALL);

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
	void* resource = nullptr;
	switch (resource_handle._type)
	{
	case RenderResource::Texture:
		resource = _textures.get_raw_ptr(resource_handle);
		break;
	case RenderResource::Buffer:
	case RenderResource::ConstantBuffer:
		resource = _buffers.get_raw_ptr(resource_handle);
		break;
	case RenderResource::PipelineState:
		resource = _pipeline_states.get_raw_ptr(resource_handle);
		break;
	case RenderResource::InstancedRenderAtom:
		resource = _render_atoms.get_raw_ptr(resource_handle);
		break;
	case RenderResource::RenderTarget:
		resource = _render_targets.get_raw_ptr(resource_handle);
		break;
	default:
		UASSERT(false, "Unknown resource type");
		break;
	}
	return reinterpret_cast<RenderResource*>(resource);
}

template<typename T>
T* allocate_resource(unsigned buffer_count)
{
	size_t required_size = sizeof(T);
	
}


RenderHandle ResourceManager::create_texture(ID3D12GraphicsCommandList *command_list, const D3D12TextureDesc &desc, const void *data)
{
	unsigned handle_count = desc.dynamic ? _render_device.back_buffer_count() : 1;

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
	unsigned required_resource_size = desc.width * desc.height * sizeof(unsigned); // Todo: actual pixel count

	auto resource_allocator = _placement_resource_heap_allocator;

	D3D12Texture texture = {};
	texture.placed_resource.allocation = resource_allocator->allocate(required_resource_size);

	HRESULT hr = device->CreatePlacedResource(resource_allocator->get_heap(texture.placed_resource.allocation),
		resource_allocator->get_offset(texture.placed_resource.allocation),
		&resource_desc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&texture.placed_resource.resource));
	UENSURE(SUCCEEDED(hr));

	// validate memory footprint
	UENSURE(GetRequiredIntermediateSize(texture.placed_resource.resource.Get(), 0, 1) == required_resource_size);

	// Setup sub resource data
	D3D12_SUBRESOURCE_DATA sub_resource_data = {};
	sub_resource_data.pData = data;
	sub_resource_data.RowPitch = desc.width * sizeof(unsigned); // TODO: use real pixel count
	sub_resource_data.SlicePitch = desc.height * sub_resource_data.RowPitch;

	// Make sure upload heap offset is properly aligned for texture upload
	_upload_heap_offset = ALIGN(_upload_heap_offset, D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT);

	// commit upload command to command list and set initial resource barrier accordingly
	UpdateSubresources(command_list, 
		texture.placed_resource.resource.Get(), 
		_upload_heap.Get(), 
		_upload_heap_offset, 
		0, 
		1, 
		&sub_resource_data);

	command_list->ResourceBarrier(1, 
		&CD3DX12_RESOURCE_BARRIER::Transition(texture.placed_resource.resource.Get(), 
			D3D12_RESOURCE_STATE_COPY_DEST, 
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE)
		);
	_upload_heap_offset += required_resource_size;

	// Setup shader resource view
	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	srv_desc.ViewDimension = srv_dimension_lut[4]; // tdodo
	srv_desc.Format = format_lut[desc.format];
	srv_desc.Texture2D.MipLevels = desc.mip_levels;
	srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	texture.srv_descriptor = _cbv_srv_uav_descriptor_allocator->allocate(handle_count);

	// create cpu descriptor handle for resource
	device->CreateShaderResourceView(texture.placed_resource.resource.Get(), 
		&srv_desc, 
		_cbv_srv_uav_descriptor_allocator->get_cpu_handle(texture.srv_descriptor));
	
	_textures.push_back(texture);
	return RenderHandle();
}

void ResourceManager::destroy_texture(RenderHandle handle)
{
}

RenderHandle ResourceManager::create_render_target(const D3D12RenderTargetDesc &desc, RenderResourceContext &rrc)
{
	ID3D12Device *device = _render_device.device();
	D3D12RenderTarget render_target = {};

	if (desc.depth_stencil) {
		HRESULT hr = device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Tex2D(format_lut[desc.format], desc.width, desc.height, 1U, 0U, 1U, 0U, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&CD3DX12_CLEAR_VALUE(format_lut[desc.format], 1.f, 0),
			IID_PPV_ARGS(&render_target.resource));
		UENSURE(SUCCEEDED(hr));

		render_target.resource_state = D3D12_RESOURCE_STATE_DEPTH_WRITE;

		render_target.dsv_descriptor = _dsv_descriptor_allocator->allocate(1);

		D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
		dsv_desc.Format = format_lut[desc.format];
		dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

		device->CreateDepthStencilView(render_target.resource.Get(), 
			&dsv_desc, 
			_dsv_descriptor_allocator->get_cpu_handle(render_target.dsv_descriptor));
	}
	else {
		const float opt_clear[] = { 0,0,0,0 };
		HRESULT hr = device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Tex2D(format_lut[desc.format], desc.width, desc.height, 1U, 0U, 1U, 0U, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			&CD3DX12_CLEAR_VALUE(format_lut[desc.format], opt_clear),
			IID_PPV_ARGS(&render_target.resource));
		UENSURE(SUCCEEDED(hr));

		render_target.rtv_descriptor = _rtv_descriptor_allocator->allocate(1);
		render_target.resource_state = D3D12_RESOURCE_STATE_RENDER_TARGET;


		D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
		rtv_desc.Format = format_lut[desc.format];
		rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		device->CreateRenderTargetView(render_target.resource.Get(), 
			&rtv_desc, 
			_rtv_descriptor_allocator->get_cpu_handle(render_target.rtv_descriptor));
	}

	_render_targets.push_back(render_target);

	return RenderHandle();
}

RenderHandle ResourceManager::create_render_target_from_resource(ID3D12Resource *resource)
{
	ID3D12Device *device = _render_device.device();
	
	D3D12RenderTarget render_target = {};
	render_target.resource_state = D3D12_RESOURCE_STATE_PRESENT;
	render_target.rtv_descriptor = _rtv_descriptor_allocator->allocate(1);
	render_target.resource = ComPtr<ID3D12Resource>(resource);

	// create cpu descriptor handle for resource
	device->CreateRenderTargetView(render_target.resource.Get(), 
		nullptr, 
		_rtv_descriptor_allocator->get_cpu_handle(render_target.rtv_descriptor));

	_render_targets.push_back(render_target);
	return RenderHandle();
}


RenderHandle ResourceManager::create_vertex_buffer(const BufferDesc &desc, const void *data, RenderResourceContext &rrc)
{
	ID3D12Device *device = _render_device.device();
	

	// Alloc gpu memory
	unsigned required_resource_size = desc.size;
	
	D3D12HeapAllocation heap_allocation = _placement_resource_heap_allocator->allocate(required_resource_size);
	ID3D12Heap* heap = _placement_resource_heap_allocator->get_heap(heap_allocation);
	D3D12HeapOffset heap_offset = _placement_resource_heap_allocator->get_offset(heap_allocation);
	ComPtr<ID3D12Resource> resource;

	HRESULT hr = device->CreatePlacedResource(heap,
		heap_offset,
		&CD3DX12_RESOURCE_DESC::Buffer(desc.size),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&resource));
	UENSURE(SUCCEEDED(hr));
	
	// validate memory footprint
	UENSURE(GetRequiredIntermediateSize(resource.Get(), 0, 1) == required_resource_size);

	// Setup sub resource data
	D3D12_SUBRESOURCE_DATA sub_resource_data = {};
	sub_resource_data.pData = data;
	sub_resource_data.RowPitch = desc.size;
		
	rrc.add_resource(resource.Get(), sub_resource_data, required_resource_size, 256, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

	// Setup vertexbuffer view
	D3D12_VERTEX_BUFFER_VIEW vbv = {};
	vbv.BufferLocation = resource->GetGPUVirtualAddress();
	vbv.SizeInBytes = desc.size;
	vbv.StrideInBytes = desc.stride;
	
	D3D12StaticBuffer buffer = {};
	buffer.resource_state = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
	buffer.placed_resource.allocation = heap_allocation;
	buffer.placed_resource.resource = resource;
	buffer.view.vbv = vbv;
	buffer.size = required_resource_size;
	

	return RenderHandle();
}

RenderHandle ResourceManager::create_index_buffer(const BufferDesc &desc, const void *data, RenderResourceContext &rrc)
{
	ID3D12Device *device = _render_device.device();

	unsigned required_resource_size = desc.size;

	D3D12HeapAllocation heap_allocation = _placement_resource_heap_allocator->allocate(required_resource_size);
	ID3D12Heap* heap = _placement_resource_heap_allocator->get_heap(heap_allocation);
	D3D12HeapOffset heap_offset = _placement_resource_heap_allocator->get_offset(heap_allocation);
	ComPtr<ID3D12Resource> resource;

	HRESULT hr = device->CreatePlacedResource(heap,
		heap_offset,
		&CD3DX12_RESOURCE_DESC::Buffer(desc.size),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&resource));
	UENSURE(SUCCEEDED(hr));

	// validate memory footprint
	UENSURE(GetRequiredIntermediateSize(resource.Get(), 0, 1) == required_resource_size);

	// Setup sub resource data
	D3D12_SUBRESOURCE_DATA sub_resource_data = {};
	sub_resource_data.pData = data;
	sub_resource_data.RowPitch = desc.size;

	rrc.add_resource(resource.Get(), sub_resource_data, required_resource_size, 256, D3D12_RESOURCE_STATE_INDEX_BUFFER);

	// Setup vertexbuffer view
	D3D12_INDEX_BUFFER_VIEW ibv = {};
	ibv.BufferLocation = resource->GetGPUVirtualAddress();
	ibv.SizeInBytes = desc.size;
	ibv.Format = (DXGI_FORMAT)desc.format;
	
	D3D12StaticBuffer buffer = {};
	buffer.placed_resource.allocation = heap_allocation;
	buffer.placed_resource.resource = resource;
	buffer.view.ibv = ibv;
	buffer.size = required_resource_size;
	buffer.resource_state = D3D12_RESOURCE_STATE_INDEX_BUFFER;

	return RenderHandle();
}

RenderHandle ResourceManager::create_constant_buffer(const BufferDesc &desc, const void *data, RenderResourceContext &rrc)
{
	ID3D12Device *device = _render_device.device();

	unsigned buffer_count = desc.dynamic ? _render_device.back_buffer_count() : 1;
	unsigned required_resource_size = ALIGN(desc.size, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
	
	D3D12ResourceAllocation resource_allocation = _cb_heap_allocator->allocate(required_resource_size * buffer_count);
	void* mapped_data = _cb_heap_allocator->get_mapped_data(resource_allocation);
	D3D12DescriptorAllocation cbv_allocation = _cbv_srv_uav_descriptor_allocator->allocate(buffer_count);
	
	for (unsigned i = 0; i < buffer_count; ++i) {
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc = {};
		cbv_desc.BufferLocation = _cb_heap_allocator->get_virtual_address(resource_allocation) + (required_resource_size * i);
		cbv_desc.SizeInBytes = required_resource_size;
		device->CreateConstantBufferView(&cbv_desc, _cbv_srv_uav_descriptor_allocator->get_cpu_handle(cbv_allocation, 0));
	}

	D3D12DynamicBuffer buffer = {};
	buffer.aligned_size = required_resource_size;
	buffer.size = desc.size;
	buffer.cbv_allocation = cbv_allocation;
	buffer.data = mapped_data;
	buffer.resource_allocation = resource_allocation;
	buffer.resource_state = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
	buffer.type = ConstantBuffer;

	return RenderHandle();
}

RenderHandle ResourceManager::create_render_atom(const InstancedRenderAtomDesc &desc, unsigned root_parameter_count, RootParameterDesc * root_parameters, RootParameterValue * root_values)
{
	size_t required_size = sizeof(InstancedRenderAtom);
	required_size += sizeof(ID3D12PipelineState*);
	required_size += sizeof(D3D12_INDEX_BUFFER_VIEW);
	required_size += sizeof(D3D12_VERTEX_BUFFER_VIEW) * desc.vertex_buffer_count;
	
	for (unsigned i = 0; i < root_parameter_count; ++i) {
		auto &root_parameter = root_parameters[i];

		switch (root_parameter.type)
		{
			case RootParameter::ROOT_CONSTANT:
			case RootParameter::ROOT_MULTIPLE_CONSTANTS:
				required_size += sizeof(RootParameter);
				required_size += sizeof(RootParameterConstants);
				required_size += sizeof(unsigned) * root_parameter.root_constant.num_values;
				break;
			case RootParameter::ROOT_CBV:
			case RootParameter::ROOT_SRV:
			case RootParameter::ROOT_UAV:
				required_size += sizeof(RootParameter);
				required_size += sizeof(D3D12_GPU_VIRTUAL_ADDRESS);
				break;
			case RootParameter::ROOT_DESCRIPTOR_TABLE:
			{
				required_size += sizeof(RootParameter);
				required_size += sizeof(RootParameterDescriptorTable);

				int j = i;
				while (root_parameters[j].index == root_parameter.index) {
					required_size += sizeof(D3D12_CPU_DESCRIPTOR_HANDLE);
					j++;
				}
				i = j - 1;
				break;
			}
		}
	}

	// check count
	InstancedRenderAtom *render_atom = resource_new<InstancedRenderAtom>(required_size);
	render_atom->size = required_size;

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

	char *tp = p;
	
	unsigned real_root_parameter_count = 0;
	for (unsigned i = 0; i < root_parameter_count; ++i) {
		auto &root_parameter = root_parameters[i];
		auto &root_value = root_values[i];

		unsigned size = sizeof(RootParameter);

		switch (root_parameter.type)
		{
			case RootParameter::ROOT_CONSTANT:
			case RootParameter::ROOT_MULTIPLE_CONSTANTS:
			{
				size += sizeof(RootParameterConstants) + sizeof(unsigned) * root_parameter.root_constant.num_values;

				auto blob_root_parameter = (RootParameter*)p;
				blob_root_parameter->size = size;
				blob_root_parameter->index = root_parameter.index;
				blob_root_parameter->type = root_parameter.type;
				blob_root_parameter->hash = -1;
			
				auto blob_root_parameter_constants = (RootParameterConstants*)(p + sizeof(RootParameter));
				blob_root_parameter_constants->num_values = root_parameter.root_constant.num_values;
				blob_root_parameter_constants->dest_offset = root_parameter.root_constant.dest_offset;
				blob_root_parameter_constants->values = (unsigned*)((char*)blob_root_parameter_constants + sizeof(RootParameterConstants));
				memcpy(blob_root_parameter_constants->values, root_value.constant_data, sizeof(unsigned)*root_parameter.root_constant.num_values);

				p += size;
				break;
			}
			case RootParameter::ROOT_CBV:
			case RootParameter::ROOT_SRV:
			case RootParameter::ROOT_UAV:
			{
				size += sizeof(D3D12_GPU_VIRTUAL_ADDRESS);

				auto blob_root_parameter = (RootParameter*)p;
				blob_root_parameter->size = size;
				blob_root_parameter->index = root_parameter.index;
				blob_root_parameter->type = root_parameter.type;
				blob_root_parameter->hash = -1;
				
				void * blob_gpu_virtual_address = (void*)(p + sizeof(RootParameter));
				RenderResource* render_resource = lookup_resource(root_value.render_resource);
				auto gpu_virtual_address = render_resource->gpu_virtual_address();
				memcpy(blob_gpu_virtual_address, &gpu_virtual_address, sizeof(D3D12_GPU_VIRTUAL_ADDRESS));

				p += size;
				break;
			}
			case RootParameter::ROOT_DESCRIPTOR_TABLE:
			{
				size += sizeof(RootParameterDescriptorTable);

				auto blob_root_parameter = (RootParameter*)p;
				blob_root_parameter->size = size;
				blob_root_parameter->index = root_parameter.index;
				blob_root_parameter->type = root_parameter.type;
				blob_root_parameter->hash = -1;
				
				auto blob_root_parameter_descriptor_table = (RootParameterDescriptorTable*)(p + sizeof(RootParameter));
				auto blob_root_parameter_descriptor_entry = (D3D12_CPU_DESCRIPTOR_HANDLE*)(p + sizeof(RootParameter) + sizeof(RootParameterDescriptorTable));
				
				unsigned j = i;
				while (root_parameters[j].index == root_parameter.index) {
					blob_root_parameter->size += sizeof(D3D12_CPU_DESCRIPTOR_HANDLE);
					blob_root_parameter_descriptor_table->num_source_descriptors++;
					RenderResource* render_resource = lookup_resource(root_values[j].render_resource);
					auto cpu_descriptor_handle = render_resource->cpu_descriptor_handle();
					memcpy(blob_root_parameter_descriptor_entry, &cpu_descriptor_handle, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
					blob_root_parameter_descriptor_entry++;
					j++;
				}
				i = j - 1;
				p += blob_root_parameter->size;
				break;
			}
		}
		real_root_parameter_count++;
	}

	render_atom->root_parameter_count = real_root_parameter_count;

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
		gps_desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		gps_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		gps_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		gps_desc.DepthStencilState.DepthEnable = true;
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