#include "fiber_system.h"
#include "job_queue.h"
#include "counter.h"
#include "wait_list.h"
#include "fiber_pool.h"
#include "fiber.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

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
namespace em
{

namespace internal
{
	JobQueue* g_jobQueues[3];
	FiberPool* g_fiberPool;
	WaitList* g_waitList;
	CounterPool* g_counterPool;

	void get_next_job(JobDeclaration** out_job_decl, Counter** out_counter)
	{
		JobDeclaration* job_decl = NULL;
		Counter* counter = NULL;
		while (job_decl == NULL) {
			//g_jobQueues[PRIORITY_HIGH]->lock();
			//g_jobQueues[PRIORITY_HIGH]->dequeue(&job_decl, &counter);
			//g_jobQueues[PRIORITY_HIGH]->unlock();

			//if (job_decl != NULL)
			//	break;

			g_jobQueues[PRIORITY_NORMAL]->lock();
			g_jobQueues[PRIORITY_NORMAL]->dequeue(&job_decl, &counter);
			g_jobQueues[PRIORITY_NORMAL]->unlock();

			//if (job_decl != NULL)
			//	break;

			//g_jobQueues[PRIORITY_LOW]->lock();
			//g_jobQueues[PRIORITY_LOW]->dequeue(&job_decl, &counter);
			//g_jobQueues[PRIORITY_LOW]->unlock();
		}

		*out_job_decl = job_decl;
		*out_counter = counter;
	}
	
	Fiber* get_fiber(JobDeclaration* job_decl, Counter* counter)
	{
		g_fiberPool->lock();
		Fiber* fiber = g_fiberPool->get_fiber(SMALL_STACK);
		fiber->func = job_decl->_job_function;
		fiber->params = job_decl->_job_params;
		fiber->counter = counter;
		g_fiberPool->unlock();
		return fiber;
	}

}


void initialize_fiber_system()
{
	internal::g_jobQueues[PRIORITY_HIGH] = new JobQueue(100);
	internal::g_jobQueues[PRIORITY_NORMAL] = new JobQueue(1000);
	internal::g_jobQueues[PRIORITY_LOW] = new JobQueue(100);
	internal::g_fiberPool = new FiberPool(128,32);
	internal::g_waitList = new WaitList(160);
	internal::g_counterPool = new CounterPool(1000);
}

void schedule_jobs(JobDeclaration* job_decls, int count, Counter** counter, unsigned priority)
{
	*counter = internal::g_counterPool->get_counter(count);
	//*counter = new Counter(count);
	internal::g_jobQueues[priority]->lock();
	for (int i = 0; i < count; ++i) {
		internal::g_jobQueues[priority]->enqueue(&job_decls[i], *counter);
	}
	internal::g_jobQueues[priority]->unlock();
}

void do_work()
{

	JobDeclaration* job_decl = NULL;
	Counter* next_counter = NULL;
	internal::get_next_job(&job_decl, &next_counter);
	Fiber* fiber = internal::get_fiber(job_decl, next_counter);

	DEBUG_FIBER_SWITCH("counter, job", fiber->fiber_handle);
	assert(fiber->fiber_handle != GetCurrentFiber() && "trying to switch to same fiber!");
	SwitchToFiber(fiber->fiber_handle);
}

void wait_for_counter(Counter* counter)
{
	void* next_fiber = NULL;
	internal::g_waitList->lock();
	internal::g_waitList->insert(counter);
	WaitEntry* waiting_fiber = internal::g_waitList->get_next_ready_fiber();
	if (waiting_fiber)
	{
		next_fiber = waiting_fiber->fiber_handle;
		waiting_fiber->fiber_handle = NULL;
		waiting_fiber->counter = NULL;
	}
	internal::g_waitList->unlock();

	if (next_fiber != NULL)
	{
		if(next_fiber != GetCurrentFiber()) {
			DEBUG_FIBER_SWITCH("counter, waiting", next_fiber);
			SwitchToFiber(next_fiber);
		} else {
			DEBUG_FIBER_SWITCH("counter, waiting no switch", GetCurrentFiber());
		}
		return;
	}

	JobDeclaration* job_decl = NULL;
	Counter* next_counter = NULL;
	internal::get_next_job(&job_decl, &next_counter);
	Fiber* fiber = internal::get_fiber(job_decl, next_counter);

	DEBUG_FIBER_SWITCH("counter, job", fiber->fiber_handle);
	assert(fiber->fiber_handle != GetCurrentFiber() && "trying to switch to same fiber!");
	SwitchToFiber(fiber->fiber_handle);
}

void release_fiber(Fiber* fiber)
{
	DEBUG_FIBER_RELASE(fiber->fiber_handle);
	internal::g_fiberPool->lock();
	fiber->counter->decrease();
	fiber->func = NULL;
	fiber->params = NULL;
	fiber->counter = NULL;
	fiber->cooldown.store(5, std::memory_order_release);
	internal::g_fiberPool->unlock();
}

void switch_fiber()
{
	void* next_fiber = NULL;

	internal::g_waitList->lock();
	WaitEntry* waiting_fiber = internal::g_waitList->get_next_ready_fiber();
	if (waiting_fiber)
	{
		next_fiber = waiting_fiber->fiber_handle;
		waiting_fiber->fiber_handle = NULL;
		waiting_fiber->counter = NULL;
	}
	internal::g_waitList->unlock();

	if (next_fiber != NULL)
	{
		if (next_fiber != GetCurrentFiber()) {
			DEBUG_FIBER_SWITCH("switch, waiting", next_fiber);
			SwitchToFiber(next_fiber);
		} else
		{
			DEBUG_FIBER_SWITCH("switch, waiting no switch", GetCurrentFiber());
		}
		return;
	}

	JobDeclaration* job_decl = NULL;
	Counter* counter = NULL;
	internal::get_next_job(&job_decl, &counter);
	Fiber* fiber = internal::get_fiber(job_decl, counter);

	if (fiber->fiber_handle != GetCurrentFiber()) {
		DEBUG_FIBER_SWITCH("switch, job", fiber->fiber_handle);
		SwitchToFiber(fiber->fiber_handle);
	}
}

}