#pragma once

namespace em
{

class Counter;

typedef void (*JobFunc)(void* params);

struct Fiber
{
	void* params;
	void* fiber_handle;
	JobFunc func;
	Counter* counter;
};

}