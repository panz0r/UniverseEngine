#include "threading.h"
#include "fiber.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace em
{

Fiber *convert_thread_to_fiber(void* params)
{
	void *fiber_handle = ConvertThreadToFiber(params);

	Fiber *fiber = new Fiber();
	fiber->fiber_handle = fiber_handle;
	fiber->func = NULL;

	return fiber;
}

void create_worker_thread(ThreadFunc func, affinity_t affinity_mask)
{
	HANDLE thread_handle = CreateThread(NULL, 0, func, NULL, CREATE_SUSPENDED, NULL);
	DWORD old_mask = SetThreadAffinityMask(thread_handle, affinity_mask);
	assert(old_mask != 0 && "failed to set affinity mask");
	ResumeThread(thread_handle);
}

}