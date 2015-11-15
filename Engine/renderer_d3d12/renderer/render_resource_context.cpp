#include "render_resource_context.h"
#include "render_device.h"
#include "d3dx12.h"

#include <core/assert/assert.h>

namespace ue
{

// Handles actual gpu data upload to placed resources via an upload heap
// Also commits transition barriers to the supplied command list

RenderResourceContext::RenderResourceContext(D3D12RenderDevice &render_device)
: _render_device(render_device)
, _upload_offset(0)
, _required_size(0)
, _upload_heap_size(0)
{
}

RenderResourceContext::~RenderResourceContext()
{
}

void RenderResourceContext::add_resource(ID3D12Resource * resource, const D3D12_SUBRESOURCE_DATA & sub_resource, unsigned size, unsigned align, D3D12_RESOURCE_STATES state_after, D3D12_RESOURCE_STATES state_before)
{
	UploadResource upload_resource =
	{
		resource,
		sub_resource,
		size,
		align,
		state_before,
		state_after
	};
	_upload_queue.push_back(upload_resource);

	// compute required size of upload heap
	_required_size = ALIGN(_required_size, align);
	_required_size += size;
}

void RenderResourceContext::dispatch(GraphicsCommandList &command_list)
{
	if (_upload_heap_size < _required_size)
	{
		allocate_upload_heap(_required_size);
	}

	ID3D12GraphicsCommandList* cmd_list = command_list.command_list();

	std::vector<D3D12_RESOURCE_BARRIER> barriers;
	barriers.reserve(_upload_queue.size());

	for (auto &resource : _upload_queue)
	{
		_upload_offset = ALIGN(_upload_offset, resource.align);
		UpdateSubresources(cmd_list, resource.resource, _upload_heap.Get(), _upload_offset, 0, 1, &resource.sub_resource_data);
		barriers.push_back(CD3DX12_RESOURCE_BARRIER::Transition(resource.resource, resource.state_before, resource.state_after));
		_upload_offset += resource.size;
	}
	cmd_list->ResourceBarrier(barriers.size(), &barriers[0]);

	_upload_queue.clear();
}

void RenderResourceContext::allocate_upload_heap(unsigned size)
{
	// upload heap should be guaranteed to not be in use here
	_upload_heap.Reset();	// release previous heap

	ID3D12Device *device = _render_device.device();

	HRESULT hr = device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(size),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_upload_heap));
	UENSURE(SUCCEEDED(hr));

	_upload_heap_size = size;
	_upload_offset = 0;
}


}