#pragma once

#include <d3d12.h>
#include "d3dx12.h"
#include <core/assert/assert.h>
#include <core/handle/handle_container.h>
#include <renderer_d3d12/com/com_ptr.h>
#include "descriptor_heap.h"
#include <vector>

namespace ue
{

enum ResourceType
{
	Texture,
	VertexBuffer
};

struct TextureDesc
{
	unsigned width;
	unsigned height;
	unsigned depth;
	unsigned mip_levels;
	unsigned format;
	unsigned dimension;
	bool dynamic;
};

struct VertexBufferDesc
{

};

// Resources, and what's needed in runtime
struct RenderResource
{
	enum Type { Texture };
	Type type;
};

struct TextureResource : public RenderResource
{
	ComPtr<ID3D12Resource> resource;
	
	DescriptorHeapHandle *handles;
	D3D12_CPU_DESCRIPTOR_HANDLE *srv;
	D3D12_CPU_DESCRIPTOR_HANDLE *uav;

	void *mapped_data;
	size_t size;
	bool dynamic;
};

struct RenderTarget
{
	enum Binding { AS_TARGET, AS_SHADER_RESOURCE };
	// srv
	// rtv/dsv
	// current state
};

struct Sampler
{
	// sampler view
	D3D12_CPU_DESCRIPTOR_HANDLE handle;
};

struct ConstantBuffer
{
	D3D12_CPU_DESCRIPTOR_HANDLE *cbv;
	void *mapped_data;
	// cbv * frames
	// mapped data * frames
	// size
};

struct StaticBuffer
{
	ID3D12Resource *resource;
	D3D12_CPU_DESCRIPTOR_HANDLE *srv;
	D3D12_CPU_DESCRIPTOR_HANDLE *uav;
	D3D12_VERTEX_BUFFER_VIEW *vbv;
	D3D12_INDEX_BUFFER_VIEW *ibv;
	void *mapped_data;

	// srv/uav/vbv/ibv
	// mapped data
	// stride
	// size
};

struct DynamicBuffer
{
	// srv/uav/vbv/ibv * frames
	// mapped data * frames
	// stride
	// size
};



typedef Handle RenderHandle;
enum { NO_RESOURCE = 0 };

class D3D12RenderDevice;

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


	void* lookup_resource(RenderHandle resource_handle);


	Handle create_texture(const TextureDesc &desc, const void *data);
	void destroy_texture(Handle handle);

	Handle create_vertex_buffer(const VertexBufferDesc &desc, const void *data);

private:
	
	
	HandleContainer<TextureResource*> _textures;

	DescriptorHeap _srv_uav_cbv_offline_heap;
	DescriptorHeap _sampler_offline_heap;

	D3D12RenderDevice&	_render_device;
	unsigned _rtv_desc_size;
	unsigned _dsv_desc_size;
	unsigned _cbv_srv_uav_desc_size;
	unsigned _sampler_desc_size;


};

}