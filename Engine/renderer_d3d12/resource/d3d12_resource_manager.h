#pragma once
#include <renderer_d3d12/stdinclude.h>
#include <renderer_d3d12/renderer/d3d12_render_device.h>

#include "d3d12_resource_storage.h"
#include "d3d12_buffer.h"
#include "d3d12_render_target.h"
#include "d3d12_root_signature.h"
#include "d3d12_pipeline_state.h"
#include "d3d12_buffer_writer.h"

namespace ue
{

struct D3D12SerializedRootSignature;


class D3D12ResourceManager
{
public:
	D3D12ResourceManager(D3D12RenderDevice& render_device);
	~D3D12ResourceManager();

	RenderHandle create_vertex_buffer(const D3D12VertexBufferDesc& vertex_buffer_desc, bool immutable);
	RenderHandle create_index_buffer(const D3D12IndexBufferDesc& index_buffer_desc, bool immutable);
	RenderHandle create_constant_buffer();
	RenderHandle create_raw_buffer();
	RenderHandle create_texture_1d();
	RenderHandle create_texture_2d();
	RenderHandle create_texture_3d();
	RenderHandle create_texture_cube();
	RenderHandle create_render_target();
	RenderHandle create_render_target_from_resource(ID3D12Resource* resource, D3D12_RESOURCE_STATES initial_resource_state);
	RenderHandle create_depth_stencil_target();

	RenderSetHandle create_root_signature(const D3D12SerializedRootSignature& serialized_root_signature);
	RenderSetHandle create_graphics_pipeline_state(const D3D12GraphicsPipelineStateDesc& pipeline_state_desc);

	inline D3D12RenderTarget* render_target(RenderHandle render_handle);
	inline D3D12VertexBuffer* vertex_buffer(RenderHandle render_handle);
	inline D3D12IndexBuffer* index_buffer(RenderHandle render_handle);
	inline D3D12ConstantBuffer* constant_buffer(RenderHandle render_handle);
	inline D3D12RootSignature* root_signature(RenderSetHandle render_handle);
	inline D3D12PipelineState* pipeline_state(RenderSetHandle render_handle);
	
	D3D12_CPU_DESCRIPTOR_HANDLE get_render_target_descriptor_handle(D3D12RenderTarget* render_target, D3D12RenderTarget::Descriptor descriptor);
	
	void buffer_writer(D3D12Buffer* buffer, D3D12BufferWriter* writer);

private:


private:
	D3D12RenderDevice& _render_device;

	RenderHandle create_immutable_vertex_buffer(const D3D12VertexBufferDesc& vertex_buffer_desc);
	RenderHandle create_mutable_vertex_buffer(const D3D12VertexBufferDesc& vertex_buffer_desc);
	RenderHandle create_immutable_index_buffer(const D3D12IndexBufferDesc& index_buffer_desc);

	ResourceStorage<D3D12RenderTarget*>	_render_targets;
	ResourceStorage<D3D12RenderTarget*>  _depth_stencil_targets;
	ResourceStorage<D3D12VertexBuffer*> _vertex_buffers;
	ResourceStorage<D3D12IndexBuffer*> _index_buffers;
	ResourceStorage<D3D12ConstantBuffer*> _constant_buffers;
	ResourceStorage<D3D12RawBuffer*> _raw_buffers;

	// Resource lookup table
	std::vector<unsigned> _resource_lookup;
	std::vector<unsigned> _free_resources;
	

	ResourceSet<D3D12RootSignature*> _root_signatures;
	ResourceSet<D3D12PipelineState*> _pipeline_states;
};

inline D3D12RenderTarget* D3D12ResourceManager::render_target(RenderHandle render_handle) {
	return _render_targets.get(extract_index(render_handle));
}
inline D3D12VertexBuffer* D3D12ResourceManager::vertex_buffer(RenderHandle render_handle) {
	return _vertex_buffers.get(extract_index(render_handle));
}
inline D3D12IndexBuffer* D3D12ResourceManager::index_buffer(RenderHandle render_handle) {
	return _index_buffers.get(extract_index(render_handle));
}
inline D3D12ConstantBuffer* D3D12ResourceManager::constant_buffer(RenderHandle render_handle) {
	return _constant_buffers.get(extract_index(render_handle));
}
inline D3D12RootSignature* D3D12ResourceManager::root_signature(RenderSetHandle render_handle) {
	return _root_signatures.get(render_handle);
}
inline D3D12PipelineState* D3D12ResourceManager::pipeline_state(RenderSetHandle render_handle) {
	return _pipeline_states.get(render_handle);
}


}