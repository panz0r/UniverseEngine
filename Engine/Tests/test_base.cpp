#include "test_base.h"
#include <Windows.h>

namespace ue
{

const char* class_name = "unit_test_window_class";

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	//case WM_CREATE:
	//	return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	/*case WM_KEYDOWN:
	{
		if (wparam == VK_ESCAPE)
			::PostQuitMessage(0);
	}
	break;*/
	}

	return ::DefWindowProc(hwnd, message, wparam, lparam);
}

int RegisterWindowClass(HINSTANCE hinstance)
{
	WNDCLASSEX wndclassex;
	{
		ZeroMemory(&wndclassex, sizeof(WNDCLASSEX));
		wndclassex.cbSize = sizeof(WNDCLASSEX);
		wndclassex.style = CS_HREDRAW | CS_VREDRAW;
		wndclassex.lpfnWndProc = WndProc;
		wndclassex.cbClsExtra = 0;
		wndclassex.cbWndExtra = 0;
		wndclassex.hInstance = hinstance;
		wndclassex.hIcon = NULL, //::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
		wndclassex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wndclassex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
		wndclassex.lpszMenuName = NULL;
		wndclassex.lpszClassName = class_name;
		wndclassex.hIconSm = wndclassex.hIcon;
	}


	if (!::RegisterClassEx(&wndclassex))
	{
		return 0;
	}
	return 1;
}

HWND CreateAppWindow(unsigned width, unsigned height, const char* window_name, HINSTANCE hinstance)
{
	HWND window_handle = ::CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		class_name,
		window_name,
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


UnitTestBase::UnitTestBase(unsigned width, unsigned height, const char* name)
{
	::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, NULL, &_hInstance);

	RegisterWindowClass(_hInstance);
	_hWnd = CreateAppWindow(width, height, name, _hInstance);
	::ShowWindow(_hWnd, 1);
}

UnitTestBase::~UnitTestBase()
{
	::ShowWindow(_hWnd, 0);
	::DestroyWindow(_hWnd);
	::UnregisterClassA(class_name, _hInstance);
}

void UnitTestBase::run()
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{ 
			int ret = execute_test();
			if (ret != S_OK)
			{
				::PostQuitMessage(0);
			}
		}
	}
}

}