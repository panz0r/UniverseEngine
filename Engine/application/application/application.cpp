#include "application.h"

#include <core/thread/threading.h>
#include <core/thread/fiber_pool.h>
#include <core/thread/scheduler.h>
#include <core/thread/job_declaration.h>

namespace em
{

Application::Application()
{}

Application::~Application()
{}

void Application::initialize()
{
	_fiber_pool = new FiberPool(64, 32);
	_scheduler = new Scheduler(_fiber_pool);
}

void test_sub_job(void* data)
{
	// Do some shit
}

void test_job(void* params)
{
	
	JobDeclaration jobs[100];
	for(unsigned i = 0; i < 100; ++i)
	{
		jobs[i] = JobDeclaration(&test_sub_job, NULL);
	}

	Counter* counter = NULL;
	schedule_jobs((JobDeclaration*)&jobs, 100, &counter);

}

void thread_entry_job(void* params)
{
	Fiber* this_fiber = convert_thread_to_fiber(params);

	FiberPool* fiber_pool = params->fiber_pool; 
	JobQueue* job_queue = params->job_queue;
	WaitList* wait_list = params->wait_list;
	
	Job* job = NULL;
	// Spin until a job pops up
	while(job_queue->dequeue(job) == false)
	{
		Sleep(1);
	}
	
	Fiber* fiber = fiber_pool->get_fiber(SMALL_STACK);
	fiber->set_job(job);
	int *counter = wait_list->put(this_fiber, 1);

	wait_for_counter(counter);

}


int Application::run()
{
	// Queue up system jobs
	_scheduler.enqueue(&thread_entry_job, PRIORITY_NORMAL);
	_scheduler.enqueue(&thread_entry_job, PRIORITY_NORMAL);
	//_scheduler.enqueue(&game_update, HIGH_PRIORITY);
	//_scheduler.enqueue(&render, HIGH_PRIORITY);

	// Frame start
	spawn_worker_thread(_scheduler, HW_THREAD_AFFINITY_0);
	spawn_worker_thread(_scheduler, HW_THREAD_AFFINITY_1);
	
	// Wait for all work to be done
	wait_for_signal(&_signal);

	// Clean up


return 0;
}

}