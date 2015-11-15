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
	IDXGISwapChain* swap_chain() { return _swap_chain.Get(); }

	unsigned current_back_buffer() { return _back_buffer_index; }
	unsigned buffer_count() const { return _desc.back_buffer_count; }
	ID3D12Resource* back_buffer(unsigned index) { return _back_buffers[index]; }

	ID3D12CommandQueue * command_queue() { return _command_queue.Get(); }

	void present();
	void wait_for_fence();

private:

	void open();
	void close();

private:
	
	D3D12RenderDeviceDesc _desc;
	unsigned _back_buffer_index;

	ComPtr<ID3D12Debug> _debug;

	// essential d3d12 interface
	ComPtr<ID3D12Device> _device;
	ComPtr<ID3D12CommandQueue> _command_queue;
	ComPtr<IDXGISwapChain3> _swap_chain;
	
	ID3D12Resource **_back_buffers;

	ComPtr<ID3D12Fence> _fence;
	HANDLE _fence_event;
	unsigned long long _fence_value;


};


}