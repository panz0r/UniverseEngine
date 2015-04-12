#pragma once

namespace em
{

struct JobParams;
class Counter;

typedef void (*JobFunc)(void* params);

struct Fiber
{
	void* params;
	void* fiber_handle;
	JobFunc func;
	Counter* counter;
	int cooldown;
};

}