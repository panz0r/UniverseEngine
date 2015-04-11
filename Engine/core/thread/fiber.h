#pragma once

namespace em
{

struct JobParams;

typedef void (*JobFunc)(void* params);

struct Fiber
{
	void* params;
	void* fiber_handle;
	JobFunc func;
};

}