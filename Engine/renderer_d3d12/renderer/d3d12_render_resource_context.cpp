#include <core/platform/platform.h>
#include <renderer_d3d12\stdinclude.h>
#include "d3d12_render_resource_context.h"
#include "d3d12_render_device.h"

#include <renderer_d3d12/memory/d3d12_memory.h>
#include <renderer_d3d12/resource/d3d12_buffer.h>
#include <renderer_d3d12/resource/d3d12_buffer_writer.h>

#include <core/assert/assert.h>

namespace ue
{

// Handles actual gpu mapped_data upload to placed resources via an upload heap
// Also commits transition barriers to the supplied command list

D3D12RenderResourceContext::D3D12RenderResourceContext(D3D12RenderDevice &render_device)
: _device(render_device)
{
}

D3D12RenderResourceContext::~D3D12RenderResourceContext()
{
}

void D3D12RenderResourceContext::update_buffer(D3D12Buffer* buffer, void* data, size_t size)
{

	if (!_resource_writer.write_buffer(buffer, data, size, 4, D3D12ResourceWriter::BUFFER))
	{
		D3D12GraphicsCommandList& command_list = _device.command_list_factory()->get_graphics_command_list(_device.current_back_buffer());
		_resource_writer.dispatch(command_list());
		command_list()->Close();
		_device.command_queue()->ExecuteCommandLists(1, command_list.command_list_cast());
		_device.command_list_factory()->executed(command_list);
		d3d12_memory::upload_ringbuffer_allocator()->wait_for_fence();
		bool fatal = _resource_writer.write_buffer(buffer, data, size, 4, D3D12ResourceWriter::BUFFER);
		UENSURE(!fatal);
	}
}

void D3D12RenderResourceContext::dispatch()
{
	D3D12GraphicsCommandList& command_list = _device.command_list_factory()->get_graphics_command_list(_device.current_back_buffer());
	_resource_writer.dispatch(command_list());
	command_list()->Close();
	_device.command_queue()->ExecuteCommandLists(1, command_list.command_list_cast());
	_device.command_list_factory()->executed(command_list);
}


}