#pragma once

#include <renderer_d3d12/stdinclude.h>
#include <core/assert/assert.h>
#include <renderer_d3d12/com/com_ptr.h>

#include <renderer_d3d12/resource/d3d12_texture.h>
#include <renderer_d3d12/resource/d3d12_render_target.h>
#include <renderer_d3d12/resource/d3d12_buffer.h>

//#include <renderer_d3d12/resource/pipeline_state_object.h>
#include <renderer_d3d12/renderer/render_resource_context.h>

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

struct RenderHandle
{
	unsigned type : 7;
	unsigned dynamic : 1;
	unsigned index : 24;
};

inline RenderHandle make_render_handle(ResourceType type, unsigned index)
{
	RenderHandle handle;
	handle.type = type;
	handle.index = index;
	return handle;
}


template <typename T>
class ResourceStorage
{
public:
	RenderHandle add_resource(const T& resource) {
		unsigned index = (unsigned)_resources.size();
		if (_free_list.size() > 0) {
			index = _free_list.back();
			_free_list.pop_back();
		}
		if (index == _resources.size())
			_resources.push_back(resource);
		else
			_resources[index] = resource;
		return make_render_handle(resource.type, index);
	}

	inline T* get_resource(unsigned index) {
		return &_resources[index];
	}

	inline void remove_resource(unsigned index) {
		_free_list.push_back(index);
	}

private:
	std::vector<T> _resources;
	std::vector<unsigned> _free_list;
};


class ResourceManager
{
public:
	ResourceManager(D3D12RenderDevice& device);
	~ResourceManager();

	D3D12RenderResource* lookup_resource(RenderHandle resource_handle);

	RenderHandle create_texture(ID3D12GraphicsCommandList *command_list, const D3D12TextureDesc &desc, const void *data);
	void destroy_texture(RenderHandle handle);

	RenderHandle create_vertex_buffer(const D3D12BufferDesc &desc, const void *data, RenderResourceContext &rrc);
	RenderHandle create_index_buffer(const D3D12BufferDesc &desc, const void *data, RenderResourceContext &rrc);
	RenderHandle create_constant_buffer(const D3D12BufferDesc &desc, const void *data, RenderResourceContext &rrc);
	RenderHandle create_render_target_from_resource(ID3D12Resource *resource);
	RenderHandle create_render_target(const D3D12RenderTargetDesc &desc, RenderResourceContext &rrc);
//	RenderHandle create_pipeline_state_object(const PipelineStateObjectDesc &desc);
//	RenderHandle create_render_atom(const InstancedRenderAtomDesc &desc, unsigned root_parameter_count, RootParameterDesc * root_parameters, RootParameterValue * root_values);

	ID3D12RootSignature* root_signature() { return _root_signature.Get(); }

public:

	D3D12DescriptorAllocator* cbv_srv_uav_allocator() { return _cbv_srv_uav_descriptor_allocator; }
	D3D12DescriptorAllocator* sampler_allocator() { return _sampler_descriptor_allocator; }
	D3D12DescriptorAllocator* rtv_allocator() { return _rtv_descriptor_allocator; }
	D3D12DescriptorAllocator* dsv_allocator() { return _dsv_descriptor_allocator; }
	
	D3D12HeapAllocator*	placement_heap_allocator() { return _placement_heap_allocator; }
	D3D12ResourceAllocator* dynamic_resource_allocator() { return _dynamic_resource_allocator; }

private:
	D3D12RenderDevice&	_render_device;

	ResourceStorage<D3D12Texture> _textures;
	ResourceStorage<D3D12RenderTarget> _render_targets;
	ResourceStorage<D3D12DynamicBuffer> _dynamic_buffers;
	ResourceStorage<D3D12StaticBuffer> _static_buffers;

	D3D12DescriptorAllocator* _cbv_srv_uav_descriptor_allocator;
	D3D12DescriptorAllocator* _sampler_descriptor_allocator;
	D3D12DescriptorAllocator* _rtv_descriptor_allocator;
	D3D12DescriptorAllocator* _dsv_descriptor_allocator;

	D3D12HeapAllocator*	_placement_heap_allocator;
	D3D12ResourceAllocator* _dynamic_resource_allocator;


	ComPtr<ID3D12Resource> _upload_heap;
	std::atomic<size_t> _upload_heap_offset;

	// Temp stuff
	ComPtr<ID3D12RootSignature> _root_signature;	// just one root signature for now
};

}