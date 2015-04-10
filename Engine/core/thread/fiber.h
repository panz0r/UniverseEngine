#pragma once

namespace em
{

struct JobParams;

typedef void (*fiber_proc)(JobParams* params);

struct Fiber
{
	void* fiber_handle;
	fiber_proc func;
};

}