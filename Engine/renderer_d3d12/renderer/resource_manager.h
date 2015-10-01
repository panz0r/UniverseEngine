#pragma once

#include <d3d12.h>
#include "d3dx12.h"
#include <renderer_d3d12/com/com_ptr.h>
#include <vector>

namespace ue
{

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

// Resources, and what's needed in runtime
struct RenderResource
{
	enum Type { Texture };
	Type type;
};

struct TextureResource : public RenderResource
{
	ComPtr<ID3D12Resource> resource;
	
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


typedef unsigned RenderHandle;
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


private:
	void create_texture(const TextureDesc &desc, const void *data);
	
	//Array<ComPtr<ID3D12Resource*> > _static_resource_heaps;
	//Array< ComPtr<ID3D12PipelineState*> > _graphics_pipeline_states;
	//Array< ComPtr<ID3D12DescriptorHeap> > _cbv_srv_uav_heaps;
	//Array< ComPtr<ID3D12DescriptorHeap> > _sampler_heaps;

	std::vector< ComPtr<ID3D12DescriptorHeap> > _srv_offline_heap;
	std::vector<TextureResource*> _textures;


	D3D12RenderDevice&	_render_device;
	unsigned _rtv_desc_size;
	unsigned _dsv_desc_size;
	unsigned _cbv_srv_uav_desc_size;
	unsigned _sampler_desc_size;


};

}