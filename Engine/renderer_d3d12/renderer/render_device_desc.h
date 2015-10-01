#pragma once

namespace ue
{



struct D3D12RenderDeviceDesc
{
	bool debug;
	HWND window_handle;
	unsigned width;
	unsigned height;
	bool fullscreen;
	unsigned back_buffer_count;
};

}