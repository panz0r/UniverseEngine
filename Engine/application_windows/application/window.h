#define WIN32_LEAN_AND_MEAN
#include <windows.h>



namespace application
{

HWND CreateAppWindow(unsigned width, unsigned height, const char* appName, const char* className);
void ShowWindow(HWND window_handle);
void HideWindow(HWND window_handle);
void DestroyWindow(HWND window_handle);

}