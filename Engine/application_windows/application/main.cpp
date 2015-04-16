#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "wndproc.h"
#include "window.h"


#include <application/application/application.h>



static const char className[] = "Universe";
static const char appName[] = "Universe";


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow)
{
	WNDCLASSEX wndclassex;
	{
		ZeroMemory(&wndclassex, sizeof(WNDCLASSEX));
		wndclassex.cbSize = sizeof(WNDCLASSEX);
		wndclassex.style = CS_HREDRAW | CS_VREDRAW;
		wndclassex.lpfnWndProc = WndProc;
		wndclassex.cbClsExtra = 0;
		wndclassex.cbWndExtra = 0;
		wndclassex.hInstance = hInstance;
		wndclassex.hIcon = NULL, //::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
		wndclassex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wndclassex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
		wndclassex.lpszMenuName = NULL;
		wndclassex.lpszClassName = className;
		wndclassex.hIconSm = wndclassex.hIcon;
	}

	if (!::RegisterClassEx(&wndclassex))
	{
		return 0;
	}
	HWND window_handle = application::CreateAppWindow(1024, 768, appName, className);
	application::ShowWindow(window_handle);

	
	
	ue::Application* application = new ue::Application(window_handle);
	application->initialize();


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
			int ret = application->run();
			if(ret != S_OK)
			{
				::PostQuitMessage(0);
			}
		}
	}

	delete application;

	application::HideWindow(window_handle);
	application::DestroyWindow(window_handle);

	::UnregisterClass(className, hInstance);

	return (int)msg.wParam;
}


