#include "render_device.h"
#include "resource_manager.h"

namespace ue
{



D3D12RenderDevice::D3D12RenderDevice(const D3D12RenderDeviceDesc & desc)
: _desc(desc)
, _back_buffer_index(0)
{
	open();
}

D3D12RenderDevice::~D3D12RenderDevice()
{
	close();
}

void D3D12RenderDevice::open()
{
	if (_desc.debug) {
		D3D12GetDebugInterface(IID_PPV_ARGS(&_debug));
		_debug->EnableDebugLayer();
	}

	HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device));
	UENSURE(SUCCEEDED(hr));

	D3D12_COMMAND_QUEUE_DESC cmdq_desc;
	cmdq_desc.NodeMask = 0;
	cmdq_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	cmdq_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdq_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	
	hr = _device->CreateCommandQueue(&cmdq_desc, IID_PPV_ARGS(&_command_queue));
	UENSURE(SUCCEEDED(hr));

	{
		ComPtr<IDXGIFactory4> factory;
		hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
		UENSURE(SUCCEEDED(hr));

		DXGI_SWAP_CHAIN_DESC swap_chain_desc = { 0 };
		swap_chain_desc.OutputWindow = _desc.window_handle;
		swap_chain_desc.BufferDesc.Width = _desc.width;
		swap_chain_desc.BufferDesc.Height = _desc.height;
		swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_desc.SampleDesc.Count = 1;
		swap_chain_desc.SampleDesc.Quality = 0;
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.BufferCount = _desc.back_buffer_count;
		swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swap_chain_desc.Flags = 0;
		swap_chain_desc.Windowed = !_desc.fullscreen;

		ComPtr<IDXGISwapChain> swap_chain;
		hr = factory->CreateSwapChain(_command_queue.Get(), &swap_chain_desc, &swap_chain);
		UENSURE(SUCCEEDED(hr));

		swap_chain.As(&_swap_chain);

		_back_buffer_index = _swap_chain->GetCurrentBackBufferIndex();

		_back_buffers = new ID3D12Resource*[_desc.back_buffer_count];
		for (unsigned i = 0; i < _desc.back_buffer_count; ++i) {
			_swap_chain->GetBuffer(i, IID_PPV_ARGS(&_back_buffers[i]));
		}
	}

	hr = _device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	_fence_value = 1;
	_fence_event = CreateEventEx(nullptr, 0, 0, EVENT_ALL_ACCESS);

}


void D3D12RenderDevice::close()
{
}


void D3D12RenderDevice::present()
{
	_swap_chain->Present(0, 0);
	_back_buffer_index = _swap_chain->GetCurrentBackBufferIndex();


}

void D3D12RenderDevice::wait_for_fence()
{
	auto fence = _fence_value;
	_command_queue->Signal(_fence.Get(), fence);
	_fence_value++;

	if (_fence->GetCompletedValue() < fence)
	{
		_fence->SetEventOnCompletion(fence, _fence_event);
		WaitForSingleObject(_fence_event, INFINITE);
	}
}


}