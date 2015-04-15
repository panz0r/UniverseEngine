#include "fiber_system.h"
#include "job_queue.h"
#include "counter.h"
#include "wait_list.h"
#include "fiber_pool.h"
#include "fiber.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define FIBER_DEBUG
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

	#define LOCK(system) system->lock()
	#define UNLOCK(system) system->unlock()

	bool get_next_job(JobDeclaration** out_job_decl, Counter** out_counter)
	{
		JobDeclaration* job_decl = NULL;
		Counter* counter = NULL;
		
		g_jobQueues[PRIORITY_HIGH]->lock();
		g_jobQueues[PRIORITY_HIGH]->dequeue(&job_decl, &counter);
		g_jobQueues[PRIORITY_HIGH]->unlock();

		if (job_decl != NULL)
			goto done;

		g_jobQueues[PRIORITY_NORMAL]->lock();
		g_jobQueues[PRIORITY_NORMAL]->dequeue(&job_decl, &counter);
		g_jobQueues[PRIORITY_NORMAL]->unlock();

		if (job_decl != NULL)
			goto done;

		g_jobQueues[PRIORITY_LOW]->lock();
		g_jobQueues[PRIORITY_LOW]->dequeue(&job_decl, &counter);
		g_jobQueues[PRIORITY_LOW]->unlock();
done:
		*out_job_decl = job_decl;
		*out_counter = counter;

		return (job_decl != NULL);
	}
	
	Fiber* get_fiber(JobDeclaration* job_decl, Counter* counter)
	{
		
		Fiber* fiber = g_fiberPool->get_fiber(SMALL_STACK);
		fiber->func = job_decl->_job_function;
		fiber->params = job_decl->_job_params;
		fiber->counter = counter;
		
		return fiber;
	}
	
	inline void release_fiber(Fiber* fiber)
	{
		if(fiber->counter != NULL)
			fiber->counter->decrease();
		fiber->func = NULL;
		fiber->params = NULL;
		fiber->counter = NULL;
	}

	void switch_fiber_and_release(Fiber* fiber)
	{
		void* next_fiber = NULL;
	
		LOCK(g_waitList);
		WaitEntry* waiting_fiber = g_waitList->get_next_ready_fiber();
		if (waiting_fiber)
		{
			next_fiber = waiting_fiber->fiber_handle;
			waiting_fiber->fiber_handle = NULL;
			waiting_fiber->counter = NULL;
		}
		UNLOCK(g_waitList);

		if (next_fiber != NULL)
		{
			if (next_fiber != GetCurrentFiber()) {
				
				DEBUG_FIBER_SWITCH("switch, waiting", next_fiber);

				LOCK(g_fiberPool);
				release_fiber(fiber);
				SwitchToFiber(next_fiber);
			} else
			{
				DEBUG_FIBER_SWITCH("switch, waiting no switch", GetCurrentFiber());
			}
			UNLOCK(g_fiberPool);
			return;
		}

		JobDeclaration* job_decl = NULL;
		Counter* counter = NULL;
		if(get_next_job(&job_decl, &counter))
		{
			LOCK(g_fiberPool);
			release_fiber(fiber);
			fiber->func = job_decl->_job_function;
			fiber->params = job_decl->_job_params;
			fiber->counter = counter;
		}
		else
		{
			LOCK(g_fiberPool);
			release_fiber(fiber);
		}
	
		UNLOCK(g_fiberPool);
		DEBUG_FIBER_SWITCH("no switch, new job", fiber->fiber_handle);
	}


	void CALLBACK fiber_proc(void* fiber_params)
	{
		// Unlock on entering new fiber
		UNLOCK(g_fiberPool);

		Fiber* fiber = (Fiber*)fiber_params;
		bool have_work = true;
		while(have_work) {

			if(fiber->func != NULL)
				fiber->func(fiber->params);

			// fiber func is done here
			switch_fiber_and_release(fiber);
		}
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
	// Temporary solution for counters
	*counter = internal::g_counterPool->get_counter(count);
	internal::g_jobQueues[priority]->lock();
	for (int i = 0; i < count; ++i) {
		internal::g_jobQueues[priority]->enqueue(&job_decls[i], *counter);
	}
	internal::g_jobQueues[priority]->unlock();
}

void do_work()
{
}

void wait_for_counter(Counter* counter)
{
	void* next_fiber = NULL;

	LOCK(internal::g_waitList);
	internal::g_waitList->insert(counter);
	WaitEntry* waiting_fiber = internal::g_waitList->get_next_ready_fiber();
	if (waiting_fiber)
	{
		next_fiber = waiting_fiber->fiber_handle;
		waiting_fiber->fiber_handle = NULL;
		waiting_fiber->counter = NULL;
	}
	UNLOCK(internal::g_waitList);

	if (next_fiber != NULL)
	{
		if(next_fiber != GetCurrentFiber()) {
			DEBUG_FIBER_SWITCH("counter, waiting", next_fiber);
			LOCK(internal::g_fiberPool);
			SwitchToFiber(next_fiber);
			
		} else {
			DEBUG_FIBER_SWITCH("counter, waiting no switch", GetCurrentFiber());
		}
		UNLOCK(internal::g_fiberPool);
		return;
	}

	JobDeclaration* job_decl = NULL;
	Counter* next_counter = NULL;
	while(!internal::get_next_job(&job_decl, &next_counter)) {;}
	
	LOCK(internal::g_fiberPool);
	Fiber* fiber = internal::get_fiber(job_decl, next_counter);

	DEBUG_FIBER_SWITCH("counter, job", fiber->fiber_handle);
	assert(fiber->fiber_handle != GetCurrentFiber() && "trying to switch to same fiber!");
	SwitchToFiber(fiber->fiber_handle);

	UNLOCK(internal::g_fiberPool);
}



}