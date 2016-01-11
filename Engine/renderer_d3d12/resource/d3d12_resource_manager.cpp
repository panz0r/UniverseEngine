#include "d3d12_resource_manager.h"
#include "d3d12_shader_factory.h"
#include <renderer_d3d12/memory/d3d12_memory.h>

#include <math/hash.h>

namespace ue
{

D3D12ResourceManager::D3D12ResourceManager(D3D12RenderDevice& render_device)
	: _render_device(render_device)
{
}

/////////////////////////////////////////////////////////

D3D12ResourceManager::~D3D12ResourceManager()
{
}



/////////////////////////////////////////////////////////

RenderHandle D3D12ResourceManager::create_vertex_buffer(const D3D12VertexBufferDesc& vertex_buffer_desc, bool immutable)
{

	if(immutable)
		return create_immutable_vertex_buffer(vertex_buffer_desc);

	return RenderHandle();
}

/////////////////////////////////////////////////////////

RenderHandle D3D12ResourceManager::create_immutable_vertex_buffer(const D3D12VertexBufferDesc& vertex_buffer_desc)
{
	ID3D12Device* device = _render_device.device();

	unsigned required_size = vertex_buffer_desc.size;
	
	D3D12Allocation allocation;
	D3D12Resource resource;
	D3D12_GPU_VIRTUAL_ADDRESS virtual_address;

	if (required_size >= 1024 * 64) {

		allocation = d3d12_memory::default_heap_allocator()->allocate(required_size);
		D3D12HeapAccess access = d3d12_memory::heap_access(allocation);
		HRESULT hr = device->CreatePlacedResource(access.heap,
			access.offset,
			&CD3DX12_RESOURCE_DESC::Buffer(required_size),
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&resource.resource));
		UENSURE(SUCCEEDED(hr));
		virtual_address = resource.resource->GetGPUVirtualAddress();
		resource.resource_state = D3D12_RESOURCE_STATE_COPY_DEST;
	}
	else {
		allocation = d3d12_memory::default_subresource_allocator()->allocate(required_size, 0U);
		D3D12SubResourceAccess access = d3d12_memory::subresource_access(allocation);
		resource = access.resource;
		virtual_address = access.virtual_address;
	}

	D3D12_VERTEX_BUFFER_VIEW vbv = {};
	vbv.BufferLocation = virtual_address;
	vbv.SizeInBytes = required_size;
	vbv.StrideInBytes = vertex_buffer_desc.stride;

	D3D12Buffer buffer;
	buffer.allocation = allocation;
	buffer.resource = resource;
	buffer.dynamic = false;


	D3D12VertexBuffer *vertex_buffer = new D3D12VertexBuffer();
	vertex_buffer->buffer = buffer;
	vertex_buffer->type = VertexBuffer;
	vertex_buffer->vbv = vbv;

	return _vertex_buffers.insert(vertex_buffer);
}

/////////////////////////////////////////////////////////

RenderHandle D3D12ResourceManager::create_mutable_vertex_buffer(const D3D12VertexBufferDesc& vertex_buffer_desc)
{
	ID3D12Device* device = _render_device.device();

	unsigned required_size = vertex_buffer_desc.size;

	D3D12Allocation allocation = d3d12_memory::upload_subresource_allocator()->allocate(required_size, 256);
	D3D12SubResourceAccess access = d3d12_memory::subresource_access(allocation);

	D3D12_VERTEX_BUFFER_VIEW vbv = {};
	vbv.BufferLocation = access.virtual_address;
	vbv.SizeInBytes = required_size;
	vbv.StrideInBytes = vertex_buffer_desc.stride;
	
	D3D12VertexBuffer* vertex_buffer = new D3D12VertexBuffer();
	vertex_buffer->buffer.allocation = allocation;
	vertex_buffer->buffer.dynamic = true;
	vertex_buffer->type = VertexBuffer;
	vertex_buffer->vbv = vbv;

	return _vertex_buffers.insert(vertex_buffer);

}


/////////////////////////////////////////////////////////


RenderHandle D3D12ResourceManager::create_index_buffer(const D3D12IndexBufferDesc& index_buffer_desc, bool immutabe)
{
	if(immutabe)
		return create_immutable_index_buffer(index_buffer_desc);

	return RenderHandle();
}

/////////////////////////////////////////////////////////

RenderHandle D3D12ResourceManager::create_immutable_index_buffer(const D3D12IndexBufferDesc& index_buffer_desc)
{
	ID3D12Device* device = _render_device.device();

	unsigned required_size = index_buffer_desc.size;

	D3D12Allocation allocation;
	D3D12Resource resource;
	D3D12_GPU_VIRTUAL_ADDRESS virtual_address;

	if (required_size >= 1024 * 64) {
		allocation = d3d12_memory::default_heap_allocator()->allocate(required_size);
		D3D12HeapAccess access = d3d12_memory::heap_access(allocation);

		HRESULT hr = device->CreatePlacedResource(access.heap,
			access.offset,
			&CD3DX12_RESOURCE_DESC::Buffer(required_size),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&resource.resource));
		UENSURE(SUCCEEDED(hr));
		virtual_address = resource.resource->GetGPUVirtualAddress();
		resource.resource_state = D3D12_RESOURCE_STATE_GENERIC_READ;
	}
	else
	{
		allocation = d3d12_memory::default_subresource_allocator()->allocate(required_size, 0U);
		D3D12SubResourceAccess access = d3d12_memory::subresource_access(allocation);
		resource = access.resource;
		virtual_address = access.virtual_address;
	}

	D3D12_INDEX_BUFFER_VIEW ibv = {};
	ibv.BufferLocation = virtual_address;
	ibv.SizeInBytes = required_size;
	ibv.Format = (DXGI_FORMAT)index_buffer_desc.format;

	D3D12Buffer buffer;
	buffer.allocation = allocation;
	buffer.resource = resource;
	buffer.dynamic = false;

	D3D12IndexBuffer* index_buffer = new D3D12IndexBuffer();
	index_buffer->buffer = buffer;
	index_buffer->type = IndexBuffer;
	index_buffer->ibv = ibv;

	return _index_buffers.insert(index_buffer);
}

/////////////////////////////////////////////////////////

RenderHandle D3D12ResourceManager::create_constant_buffer()
{
	UE_NO_IMPL;
	return RenderHandle();
}

/////////////////////////////////////////////////////////

RenderHandle D3D12ResourceManager::create_raw_buffer()
{
	UE_NO_IMPL;
	return RenderHandle();
}

/////////////////////////////////////////////////////////

RenderHandle D3D12ResourceManager::create_texture_1d()
{
	UE_NO_IMPL;
	return RenderHandle();
}

/////////////////////////////////////////////////////////

RenderHandle D3D12ResourceManager::create_texture_2d()
{
	UE_NO_IMPL;
	return RenderHandle();
}

/////////////////////////////////////////////////////////

RenderHandle D3D12ResourceManager::create_texture_3d()
{
	UE_NO_IMPL;
	return RenderHandle();
}

/////////////////////////////////////////////////////////

RenderHandle D3D12ResourceManager::create_texture_cube()
{
	UE_NO_IMPL;
	return RenderHandle();
}

/////////////////////////////////////////////////////////

RenderHandle D3D12ResourceManager::create_render_target()
{
	UE_NO_IMPL;
	return RenderHandle();
}

/////////////////////////////////////////////////////////

RenderHandle D3D12ResourceManager::create_render_target_from_resource(ID3D12Resource* resource, D3D12_RESOURCE_STATES initial_resource_state)
{
	ID3D12Device *device = _render_device.device();
	D3D12DescriptorAllocator* allocator = static_cast<D3D12DescriptorAllocator*>(d3d12_memory::allocator(ALLOCATOR_RTV_DESCRIPTOR_HEAP));

	D3D12_RESOURCE_DESC desc = resource->GetDesc();
	D3D12RenderTarget* render_target = new D3D12RenderTarget();
	render_target->resource_state = initial_resource_state;
	render_target->rtv_allocation = allocator->allocate(1);
	render_target->resource = ComPtr<ID3D12Resource>(resource);
	render_target->type = RenderTarget;
	render_target->format = (unsigned)desc.Format;

	// create cpu descriptor handle for resource
	device->CreateRenderTargetView(render_target->resource.Get(),
		nullptr,
		allocator->get_cpu_handle(render_target->rtv_allocation));

	return _render_targets.insert(render_target);

}


/////////////////////////////////////////////////////////

RenderHandle D3D12ResourceManager::create_depth_stencil_target()
{
	UE_NO_IMPL;
	return RenderHandle();
}

/////////////////////////////////////////////////////////

RenderSetHandle D3D12ResourceManager::create_root_signature(const D3D12SerializedRootSignature& serialized_root_signature)
{
	unsigned root_signature_hash = serialized_root_signature.hash;
	if (_root_signatures.contains(root_signature_hash)) {
		return make_render_handle(root_signature_hash);
	}

	ID3D12Device* device = _render_device.device();
	ComPtr<ID3D12RootSignature> root_signature;
	HRESULT hr = device->CreateRootSignature(0, serialized_root_signature.blob->GetBufferPointer(), serialized_root_signature.blob->GetBufferSize(), IID_PPV_ARGS(&root_signature));
	if(FAILED(hr))
		return RenderSetHandle();

	D3D12RootSignature* sig = new D3D12RootSignature();
	sig->signature = root_signature;

	RenderSetHandle handle = _root_signatures.insert(root_signature_hash, sig );
	return handle;
}

/////////////////////////////////////////////////////////

RenderSetHandle D3D12ResourceManager::create_graphics_pipeline_state(const D3D12GraphicsPipelineStateDesc& pipeline_state_desc)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
	desc.InputLayout = pipeline_state_desc.input_layout;
	desc.pRootSignature = pipeline_state_desc.root_signature.signature.Get();
	
	ID3DBlob* vs = pipeline_state_desc.shaders[VertexShader].blob.Get();
	ID3DBlob* ps = pipeline_state_desc.shaders[PixelShader].blob.Get();

	if(vs)
		desc.VS = { vs->GetBufferPointer(), vs->GetBufferSize() };
	if (ps)
		desc.PS = { ps->GetBufferPointer(), ps->GetBufferSize() };

	desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	desc.DepthStencilState.DepthEnable = false; // Temp

	desc.SampleMask = 0xffffffff;
	desc.SampleDesc.Count = 1;

	desc.PrimitiveTopologyType = (D3D12_PRIMITIVE_TOPOLOGY_TYPE)pipeline_state_desc.primitive_topology_type;

	for (unsigned i = 0; i < pipeline_state_desc.render_target_count; ++i) {
		desc.RTVFormats[i] = (DXGI_FORMAT)pipeline_state_desc.render_target_formats[i];
	}
	desc.NumRenderTargets = pipeline_state_desc.render_target_count;
	desc.DSVFormat = (DXGI_FORMAT)pipeline_state_desc.depth_stencil_format;

	D3D12PipelineState* pipeline_state = new D3D12PipelineState();
	pipeline_state->root_signature = pipeline_state_desc.root_signature;

	ID3D12Device* device = _render_device.device();
	HRESULT hr = device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipeline_state->pipeline_state));
	
	unsigned hash = default_hash(&desc, sizeof(desc), 0);
	
	RenderSetHandle handle = _pipeline_states.insert(hash, pipeline_state);
	return handle;
}


/////////////////////////////////////////////////////////

D3D12_CPU_DESCRIPTOR_HANDLE D3D12ResourceManager::get_render_target_descriptor_handle(D3D12RenderTarget* render_target, D3D12RenderTarget::Descriptor descriptor)
{
	switch (descriptor)
	{
	case D3D12RenderTarget::SRV_DESCRIPTOR:
	{
		auto* allocator = static_cast<D3D12DescriptorAllocator*>(d3d12_memory::allocator(ALLOCATOR_SRV_CBV_UAV_DESCRIPTOR_HEAP));
		return allocator->get_cpu_handle(render_target->srv_allocation);
	}
	case D3D12RenderTarget::UAV_DESCRIPTOR:
	{
		auto* allocator = static_cast<D3D12DescriptorAllocator*>(d3d12_memory::allocator(ALLOCATOR_SRV_CBV_UAV_DESCRIPTOR_HEAP));
		return allocator->get_cpu_handle(render_target->uav_allocation);
	}
	case D3D12RenderTarget::RTV_DESCRIPTOR:
	{
		auto* allocator = static_cast<D3D12DescriptorAllocator*>(d3d12_memory::allocator(ALLOCATOR_RTV_DESCRIPTOR_HEAP));
		return allocator->get_cpu_handle(render_target->rtv_allocation);
	}
	case D3D12RenderTarget::DSV_DESCRIPTOR:
	{
		auto* allocator = static_cast<D3D12DescriptorAllocator*>(d3d12_memory::allocator(ALLOCATOR_DSV_DESCRIPTOR_HEAP));
		return allocator->get_cpu_handle(render_target->dsv_allocation);
	}
	default:
		UE_NO_IMPL;
		return D3D12_CPU_DESCRIPTOR_HANDLE();
	}
}

/////////////////////////////////////////////////////////

void D3D12ResourceManager::buffer_writer(D3D12Buffer* buffer, D3D12BufferWriter* writer)
{
	writer = new (writer) D3D12BufferWriter(buffer);
}

}