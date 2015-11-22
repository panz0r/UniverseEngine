#pragma once

#include <d3d12.h>
#include "d3dx12.h"
#include <core/assert/assert.h>
#include <core/handle/handle_container.h>
#include <core/memory/buddy_allocator_ext.h>

#include <renderer_d3d12/com/com_ptr.h>

#include <renderer_d3d12/resource/texture.h>
#include <renderer_d3d12/resource/buffer.h>
#include <renderer_d3d12/resource/pipeline_state_object.h>
#include <renderer_d3d12/renderer/render_resource_context.h>

#include "offline_descriptor_heap.h"
#include <vector>
#include <atomic>



namespace ue
{

enum { NO_RESOURCE = 0 };

class D3D12RenderDevice;
struct RenderAtom;
struct InstancedRenderAtomDesc;
struct RootParameterDesc;
struct RootParameterValue;

class ResourceManager
{
public:
	enum { TYPE_MASK = 0xff000000, TYPE_SHIFT = 24, INDEX_MASK = 0x00ffffff };
public: 
	ResourceManager(D3D12RenderDevice& device);
	~ResourceManager();

	inline unsigned extract_type(unsigned handle)
	{
		return ((handle & TYPE_MASK) >> TYPE_SHIFT);
	}

	inline unsigned extract_index(unsigned handle)
	{
		return (handle & INDEX_MASK);
	}

	RenderResource* lookup_resource(RenderHandle resource_handle);

	RenderHandle create_texture(ID3D12GraphicsCommandList *command_list, const TextureDesc &desc, const void *data);
	void destroy_texture(RenderHandle handle);

	RenderHandle create_vertex_buffer(const BufferDesc &desc, const void *data, RenderResourceContext &rrc);
	RenderHandle create_index_buffer(const BufferDesc &desc, const void *data, RenderResourceContext &rrc);
	RenderHandle create_constant_buffer(const BufferDesc &desc, const void *data, RenderResourceContext &rrc);
	RenderHandle create_render_target_from_resource(ID3D12Resource *resource);
	RenderHandle create_render_target(const RenderTargetDesc &desc, RenderResourceContext &rrc);
	RenderHandle create_pipeline_state_object(const PipelineStateObjectDesc &desc);
	RenderHandle create_render_atom(const InstancedRenderAtomDesc &desc, unsigned root_parameter_count, RootParameterDesc * root_parameters, RootParameterValue * root_values);

	ID3D12RootSignature* root_signatire() { return _root_signature.Get(); }

private:
	D3D12RenderDevice&	_render_device;
	
	HandleContainer<TextureResource*> _textures;
	HandleContainer<RenderTargetResource*> _render_targets;
	HandleContainer<BufferResource*> _buffers;
	HandleContainer<PipelineStateResource*> _pipeline_states;
	HandleContainer<RenderAtom*> _render_atoms;

	OfflineDescriptorHeap _srv_uav_cbv_offline_heap;
	OfflineDescriptorHeap _frame_srv_uav_cbv_offline_heap;
	OfflineDescriptorHeap _sampler_offline_heap;
	OfflineDescriptorHeap _rtv_heap;
	OfflineDescriptorHeap _dsv_heap;

	ComPtr<ID3D12Heap> _resource_heap;

	ComPtr<ID3D12Resource> _upload_heap;
	std::atomic<size_t> _upload_heap_offset;


	ComPtr<ID3D12Resource> _cb_heap;
	std::atomic<size_t> _cb_heap_offset;
	void *				_cb_mapped_data;

	BuddyAllocatorExt _resource_allocator;

	// Temp stuff
	ComPtr<ID3D12RootSignature> _root_signature;	// just one root signature for now
	


	unsigned _rtv_desc_size;
	unsigned _dsv_desc_size;
	unsigned _cbv_srv_uav_desc_size;
	unsigned _sampler_desc_size;


};

}