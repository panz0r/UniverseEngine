#pragma once

namespace em
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