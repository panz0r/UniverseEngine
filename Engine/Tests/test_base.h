#pragma once
#include <Windows.h>

namespace ue
{

struct UnitTestDesc
{
	unsigned width, height;
	const char* test_name;
};

class UnitTestBase
{
public:
	UnitTestBase(unsigned width, unsigned height, const char* name);
	virtual ~UnitTestBase();
	void run();

protected:
	virtual int execute_test() = 0;
protected:
	HWND _hWnd;
	HINSTANCE _hInstance;
};

}