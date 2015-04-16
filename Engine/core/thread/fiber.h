#pragma once

#if defined(WIN32)
#include "win32/fiber_win32.h"
#else
#error "Unsupported Platform"
#endif

namespace ue
{

class Counter;

typedef void (*JobFunc)(void* params);

struct Fiber
{
	bool in_use;
	void* params;
	void* fiber_handle;
	JobFunc func;
	Counter* counter;
};

}