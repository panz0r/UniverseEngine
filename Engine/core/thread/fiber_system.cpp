#include "fiber_system.h"
#include "fiber.h"
#include "thread.h"

//#define FIBER_DEBUG
#if defined(FIBER_DEBUG)
#define DEBUG_FIBER_SWITCH(tag, fiber) \
	char temp_buffer[128]; \
	sprintf(temp_buffer, "(%d): (%s), From: %08x, To: %08x\n", GetCurrentThreadId(), tag, GetCurrentFiber(), fiber); \
	OutputDebugString(temp_buffer); \
	assert(fiber != NULL);

#define DEBUG_FIBER_RELASE(fiber) \
	char temp_buffer[128]; \
	sprintf(temp_buffer, "(%d): (release): %08x\n", GetCurrentThreadId(), fiber); \
	OutputDebugString(temp_buffer);
#else
#define DEBUG_FIBER_SWITCH(tag, fiber)
#define DEBUG_FIBER_RELASE(fiber)
#endif

namespace ue
{


///////////////////////////////////////////////////////////////////////////////////

__declspec(thread)	void *tls_current_fiber;
__declspec(thread)	void *tls_next_fiber;
__declspec(thread)	void *tls_counter;
__declspec(thread) unsigned tls_thread_id;


FiberSystem::FiberSystem()
{
}


FiberSystem::~FiberSystem()
{
}

void FiberSystem::initialize(unsigned thread_count, unsigned fiber_pool_count)
{
	_threads = new void*[thread_count];
	_switching_fibers = new void*[thread_count];
	_counter_waiting_fibers = new void*[thread_count];

	const unsigned utility_fiber_stack_size = 32 * 1024;
	for (unsigned i = 0; i < thread_count; ++i) {
		_switching_fibers[i] = create_fiber(utility_fiber_stack_size, SwitchFiberFunction, this);
		_counter_waiting_fibers[i] = create_fiber(utility_fiber_stack_size, WaitOnCounterFiberFunction, this);
	}

	_threads[0] = get_current_thread();
	set_thread_affinity_mask(_threads[0], 1);
	tls_thread_id = 0;

	for (unsigned i = 1; i < thread_count; ++i)
	{
		_threads[i] = create_worker_thread(ThreadFunction, (1 << i), this);
	}
	
	convert_thread_to_fiber(nullptr);

}

unsigned long __stdcall FiberSystem::ThreadFunction(void *params)
{
	ThreadParams* thread_params = (ThreadParams*)params;
	tls_thread_id = thread_params->thread_id;
	FiberSystemParams* fiber_system_params = thread_params->thread_system_params;

	convert_thread_to_fiber(nullptr);

	FiberFunction(fiber_system_params);

	convert_fiber_to_thread();

	return 1;	
}

void __stdcall FiberSystem::FiberFunction(void *params)
{

}

void __stdcall FiberSystem::SwitchFiberFunction(void *params)
{
	FiberSystem *fiber_system = (FiberSystem*)params;
	while (true)
	{

	}
}

void __stdcall FiberSystem::WaitOnCounterFiberFunction(void *params)
{
	FiberSystem *fiber_system = (FiberSystem*)params;
}





}