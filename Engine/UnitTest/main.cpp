#include <Windows.h>

const char* test_dll = "tests.dll";

typedef void (*run_tests_func)(void);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow)
{
	HMODULE module = LoadLibraryA(test_dll);
	run_tests_func run_tests = (run_tests_func)::GetProcAddress(module, "run_tests");
	run_tests();
	FreeLibrary(module);
	return 0;
}