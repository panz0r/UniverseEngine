#include "render_device.h"
#include <assert.h>

namespace ue
{

D3D11RenderDevice::D3D11RenderDevice(HWND window_handle, unsigned width, unsigned height)
{

	D3D_FEATURE_LEVEL feature_levels[] = 
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	DXGI_SWAP_CHAIN_DESC swap_chain_desc = {0};
	swap_chain_desc.BufferDesc.Width = width;
	swap_chain_desc.BufferDesc.Height = height;
	swap_chain_desc.BufferDesc.Format =  DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.BufferCount = 2;
	swap_chain_desc.OutputWindow = window_handle;
	swap_chain_desc.Windowed = TRUE;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;

	D3D_FEATURE_LEVEL feature_level;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		(const D3D_FEATURE_LEVEL*)&feature_levels, sizeof(feature_levels) / sizeof(D3D_FEATURE_LEVEL),
		D3D11_SDK_VERSION,
		&swap_chain_desc,
		&_swap_chain,
		&_device,
		&feature_level,
		&_immediate_context
		);

	assert(SUCCEEDED(hr));

}

D3D11RenderDevice::~D3D11RenderDevice()
{

}

}
