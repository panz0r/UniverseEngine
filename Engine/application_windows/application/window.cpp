#include "window.h"

namespace application
{

HWND CreateAppWindow(unsigned width, unsigned height, const char* appName, const char* className)
{
	HINSTANCE hinstance;
	if (!::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, NULL, &hinstance))
		return NULL;
	
	HWND window_handle = ::CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		className,
		appName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		NULL,
		NULL,
		hinstance,
		NULL);

	return window_handle;
}

void ShowWindow(HWND window_handle)
{
	::ShowWindow(window_handle, 1);
}

void HideWindow(HWND window_handle)
{
	::ShowWindow(window_handle, 0);
}

void DestroyWindow(HWND window_handle)
{
	::DestroyWindow(window_handle);
}
}