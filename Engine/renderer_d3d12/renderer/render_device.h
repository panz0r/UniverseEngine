#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>

#include <renderer_d3d12/com/com_ptr.h>
#include "render_device_desc.h"

namespace ue
{

class ResourceManager;

class D3D12RenderDevice
{
public:
	D3D12RenderDevice(const D3D12RenderDeviceDesc& desc);
	~D3D12RenderDevice();

	ID3D12Device* device() { return _device.Get(); }
	unsigned buffer_count() const { return _desc.back_buffer_count; }

private:

	void open();
	void close();

private:

	ResourceManager* _resource_manager;

	D3D12RenderDeviceDesc _desc;
	unsigned _back_buffer_index;

	ComPtr<ID3D12Debug> _debug;

	// essential d3d12 interface
	ComPtr<ID3D12Device> _device;
	ComPtr<ID3D12CommandQueue> _command_queue;
	ComPtr<IDXGISwapChain3> _swap_chain;
};


}