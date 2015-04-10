#include "threading.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace em
{

Fiber *convert_thread_to_fiber(JobParams* params)
{
	void *fiber_handle = ConvertThreadToFiber(params);

	Fiber *fiber = new Fiber();
	fiber->fiber_handle = fiber_handle;
	fiber->func = NULL;

	return fiber;
}

}