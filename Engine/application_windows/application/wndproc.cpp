#include "wndproc.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_CREATE:
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		{
			if(wparam == VK_ESCAPE)
				::PostQuitMessage(0);
		}
		break;
	}

	return ::DefWindowProc(hwnd, message, wparam, lparam);
}