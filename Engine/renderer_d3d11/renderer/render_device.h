#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <d3d11_1.h>

namespace ue
{

class D3D11RenderDevice
{
public:
	D3D11RenderDevice(HWND window_handle, unsigned width, unsigned height);
	~D3D11RenderDevice();

private:
	ID3D11Device* _device;
	ID3D11DeviceContext* _immediate_context;
	IDXGISwapChain* _swap_chain;
	unsigned _width;
	unsigned _height;
};


}