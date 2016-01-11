#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>

#include <renderer\renderer\render_device_interface.h>

#include <renderer_d3d12/com/com_ptr.h>
#include "render_device_desc.h"

namespace ue
{

class D3D12ResourceManager;
class D3D12CommandListFactory;
class D3D12RenderResourceContext;

class D3D12RenderDevice : public IRenderDevice
{
public:
	D3D12RenderDevice(const RenderDeviceDesc& desc);
	virtual ~D3D12RenderDevice();

	// Interface exposed methods

	virtual void open();
	virtual void close();
	virtual unsigned current_back_buffer() { 
		return _back_buffer_index; 
	}
	virtual unsigned back_buffer_count() {
		return _render_device_desc.back_buffer_count;
	}
	virtual void present();
	virtual void wait_for_fence();

	// D3D12 internal methods

	ID3D12Device * device() { return _device.Get(); }
	IDXGISwapChain * swap_chain() { return _swap_chain.Get(); }
	ID3D12Resource * get_back_buffer(unsigned index) { return _back_buffers[index]; }
	ID3D12CommandQueue * command_queue() { return _command_queue.Get(); }

	D3D12CommandListFactory* command_list_factory() { return _command_list_factory; }
	D3D12ResourceManager* resource_manager() { return _resource_manager; }

	D3D12RenderResourceContext* new_render_resource_context();
	void destroy_render_resource_context(D3D12RenderResourceContext* rrc);

private:
	RenderDeviceDesc _render_device_desc;
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


	D3D12CommandListFactory* _command_list_factory;
	D3D12ResourceManager* _resource_manager;
};


}