#include "scheduler.h"
#include "job_queue.h"
#include "counter.h"
#include "wait_list.h"
#include "fiber_pool.h"
#include "fiber.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace em
{

namespace internal
{
	Scheduler *g_scheduler = NULL;
}


Scheduler::Scheduler(FiberPool* fiber_pool)
 : _fiber_pool(fiber_pool)
{
	_job_queues[PRIORITY_HIGH] = new JobQueue(500);
	_job_queues[PRIORITY_NORMAL] = new JobQueue(1000);
	_job_queues[PRIORITY_LOW] = new JobQueue(500);
}

void Scheduler::schedule_jobs(JobDeclaration* job_decls, int count, Counter* counter, unsigned priortiy)
{
	_job_queues[priortiy]->lock();
	for(int i = 0; i < count; ++i) {
		_job_queues[priortiy]->enqueue(&job_decls[i], counter);
	}
	_job_queues[priortiy]->unlock();
}


void Scheduler::add_to_wait_list_and_switch_fiber(Counter* counter)
{
	void* next_fiber = NULL;

	_wait_list->lock();
	_wait_list->insert(counter);
	WaitEntry* waiting_fiber = _wait_list->get_next_ready_fiber();
	if(waiting_fiber)
	{
		next_fiber = waiting_fiber->fiber_handle;
		waiting_fiber->fiber_handle = NULL;
		waiting_fiber->counter = NULL;
	}
	_wait_list->unlock();

	if(next_fiber != NULL && next_fiber != GetCurrentFiber())
	{
		SwitchToFiber(next_fiber);
		return;
	}

	JobDeclaration* job_decl = NULL;
	Counter* next_counter = NULL;

	get_next_job(&job_decl, &next_counter);
	Fiber* fiber = get_fiber(job_decl, next_counter);

	SwitchToFiber(fiber->fiber_handle);
}


void Scheduler::release_fiber(Fiber* fiber)
{
	JobDeclaration* job_decl = NULL;
	Counter* counter = NULL;
	get_next_job(&job_decl, &counter);

	_fiber_pool->lock();

	_fiber_pool->unlock();
}

void Scheduler::get_next_job(JobDeclaration** out_job_decl, Counter** out_counter)
{
	JobDeclaration* job_decl = NULL;
	Counter* counter = NULL;
	while (job_decl == NULL) {
		_job_queues[PRIORITY_HIGH]->lock();
		_job_queues[PRIORITY_HIGH]->dequeue(&job_decl, &counter);
		_job_queues[PRIORITY_HIGH]->unlock();

		if (job_decl != NULL)
			break;

		_job_queues[PRIORITY_NORMAL]->lock();
		_job_queues[PRIORITY_NORMAL]->dequeue(&job_decl, &counter);
		_job_queues[PRIORITY_NORMAL]->unlock();

		if (job_decl != NULL)
			break;

		_job_queues[PRIORITY_LOW]->lock();
		_job_queues[PRIORITY_LOW]->dequeue(&job_decl, &counter);
		_job_queues[PRIORITY_LOW]->unlock();
	}

	*out_job_decl = job_decl;
	*out_counter = counter;
}

Fiber* Scheduler::get_fiber(JobDeclaration* job_decl, Counter* counter)
{
	_fiber_pool->lock();
	Fiber* fiber = _fiber_pool->get_fiber(SMALL_STACK);
	fiber->func = job_decl->_job_function;
	fiber->params = job_decl->_job_params;
	_fiber_pool->unlock();
	return fiber;
}


void create_scheduler(FiberPool* fiber_pool)
{
	internal::g_scheduler = new Scheduler(fiber_pool);
}

void schedule_jobs(JobDeclaration* jobs, int count, Counter** counter, unsigned priority)
{
	*counter = new Counter(count);
	internal::g_scheduler->schedule_jobs(jobs, count, *counter, priority);
}


void wait_for_counter(Counter* counter)
{
	internal::g_scheduler->add_to_wait_list_and_switch_fiber(counter);
}

void release_fiber(Fiber* fiber)
{
	internal::g_scheduler->release_fiber(fiber);
}

}