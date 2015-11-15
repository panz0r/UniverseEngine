#pragma once

#include <d3d12.h>
#include "command_list.h"
#include <vector>

namespace ue
{

#if !defined(ALIGN)
#define ALIGN(x, align) (((x) + (align-1)) & ~(align-1))
#endif

class D3D12RenderDevice;

class RenderResourceContext
{
public:
	RenderResourceContext(D3D12RenderDevice& render_device);
	~RenderResourceContext();

	void add_resource(ID3D12Resource* resource, const D3D12_SUBRESOURCE_DATA &sub_resource, unsigned size, unsigned align, D3D12_RESOURCE_STATES state_after, D3D12_RESOURCE_STATES state_before = D3D12_RESOURCE_STATE_COPY_DEST);
	void dispatch(GraphicsCommandList &command_list);

private:
	void allocate_upload_heap(unsigned size);

	struct UploadResource
	{
		ID3D12Resource *resource;
		D3D12_SUBRESOURCE_DATA sub_resource_data;
		unsigned size;
		unsigned align;
		D3D12_RESOURCE_STATES state_before;
		D3D12_RESOURCE_STATES state_after;
	};

	std::vector<UploadResource> _upload_queue;
	
	D3D12RenderDevice &_render_device;
	ComPtr<ID3D12Resource> _upload_heap;
	unsigned _upload_heap_size;
	unsigned _upload_offset;
	unsigned _required_size;
	
};

}