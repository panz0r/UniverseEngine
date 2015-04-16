#include "fiber_system.h"
#include "job_queue.h"
#include "counter.h"
#include "wait_list.h"
#include "fiber_pool.h"

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
		fiber->in_use = true;
		if(job_decl != NULL)
		{
			fiber->func = job_decl->_job_function;
			fiber->params = job_decl->_job_params;
			fiber->counter = counter;
		}

		return fiber;
	}
	
	inline void decrease_counter(Fiber* fiber)
	{
		if(fiber->counter != NULL)
			fiber->counter->decrease();
	}

	inline void release_fiber(Fiber* fiber)
	{
		fiber->func = NULL;
		fiber->params = NULL;
		fiber->counter = NULL;
		fiber->in_use = false;
	}

	void switch_fiber_and_release(Fiber* fiber)
	{
		
	
		LOCK(g_waitList);
		void* waiting_fiber_handle = NULL;
		WaitEntry* waiting_fiber = g_waitList->get_ready_fiber();
		if (waiting_fiber)
		{
			waiting_fiber_handle = waiting_fiber->fiber_handle;
			waiting_fiber->fiber_handle = NULL;
			waiting_fiber->counter = NULL;
		}
		UNLOCK(g_waitList);

		if (waiting_fiber_handle != NULL)
		{
			if (waiting_fiber_handle != get_current_fiber()) {
				
				DEBUG_FIBER_SWITCH("switch, waiting", next_fiber);

				LOCK(g_fiberPool);
				decrease_counter(fiber);
				release_fiber(fiber);
				switch_to_fiber(waiting_fiber_handle);
			} else
			{
				DEBUG_FIBER_SWITCH("switch, waiting no switch", GetCurrentFiber());
			}
			UNLOCK(g_fiberPool);
			return;
		}

		JobDeclaration* job_decl = NULL;
		Counter* counter = NULL;
		get_next_job(&job_decl, &counter);

		LOCK(g_fiberPool);
		decrease_counter(fiber);
		fiber->func = job_decl != NULL ? job_decl->_job_function : NULL;
		fiber->params = job_decl != NULL ? job_decl->_job_params : NULL;
		fiber->counter = counter;
		UNLOCK(g_fiberPool);
		
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
	internal::g_jobQueues[PRIORITY_NORMAL] = new JobQueue(5000);
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
	// get pending waiting fiber that is ready to be rescheduled
	LOCK(internal::g_waitList);
	void* waiting_fiber_handle = NULL;
	WaitEntry* waiting_fiber = internal::g_waitList->get_ready_fiber();
	if (waiting_fiber) 
	{
		waiting_fiber_handle = waiting_fiber->fiber_handle;
		waiting_fiber->fiber_handle = NULL;
		waiting_fiber->counter = NULL;
	}
	UNLOCK(internal::g_waitList);

	if(waiting_fiber_handle != NULL) 
	{
		if(waiting_fiber_handle != get_current_fiber())
		{
			LOCK(internal::g_fiberPool);
			switch_to_fiber(waiting_fiber_handle);
		}
		UNLOCK(internal::g_fiberPool);
		return;
	}

	// Attempt to get a job for the job queue and pair it with a fiber
	JobDeclaration* job_decl = NULL;
	Counter* next_counter = NULL;
	if(internal::get_next_job(&job_decl, &next_counter))
	{
		LOCK(internal::g_fiberPool);
		Fiber* fiber = internal::get_fiber(job_decl, next_counter);

		DEBUG_FIBER_SWITCH("counter, job", fiber->fiber_handle);
		switch_to_fiber(fiber->fiber_handle);

		UNLOCK(internal::g_fiberPool);
	}

}

void wait_for_counter(Counter* counter)
{
	
	JobDeclaration* job_decl = NULL;
	Counter* next_counter = NULL;
	internal::get_next_job(&job_decl, &next_counter);
	LOCK(internal::g_fiberPool);
	Fiber* fiber = internal::get_fiber(job_decl, next_counter);
	DEBUG_FIBER_SWITCH("counter, job", fiber->fiber_handle);
	assert(fiber->fiber_handle != GetCurrentFiber() && "trying to switch to same fiber!");

	// Note to self. This can be a source of crashes. 
	// All jobs this fiber depends on could theoretically
	// complete before this fiber is switched out
	// resulting in a possible switch-in of the same fiber
	// from another fiber. chance of this happening is tiny, but Murphy says it will happen.
	LOCK(internal::g_waitList);
	internal::g_waitList->insert(counter, GetCurrentFiber());
	UNLOCK(internal::g_waitList);

	switch_to_fiber(fiber->fiber_handle);
	UNLOCK(internal::g_fiberPool);
}



}