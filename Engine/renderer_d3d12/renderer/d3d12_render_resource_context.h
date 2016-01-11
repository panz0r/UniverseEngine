#pragma once

#include <d3d12.h>
#include "d3d12_command_list_factory.h"
#include "../resource/d3d12_buffer_writer.h"
#include <vector>

namespace ue
{

#if !defined(ALIGN)
#define ALIGN(x, align) (((x) + (align-1)) & ~(align-1))
#endif

class D3D12RenderDevice;
struct D3D12Buffer;

class D3D12RenderResourceContext
{
public:
	D3D12RenderResourceContext(D3D12RenderDevice& render_device);
	~D3D12RenderResourceContext();

	void update_buffer(D3D12Buffer* buffer, void* data, size_t size);
	void dispatch();

private:
	D3D12ResourceWriter _resource_writer;
	D3D12RenderDevice &_device;	
};

}