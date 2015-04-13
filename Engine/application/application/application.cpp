#include "application.h"

#include <core/thread/threading.h>
#include <core/thread/fiber_system.h>
#include <core/thread/job_declaration.h>

#include <atomic>

namespace em
{

std::atomic<int> global_counter;
std::atomic<int> global_counter2;

Application::Application()
{}

Application::~Application()
{}

void Application::initialize()
{
	initialize_fiber_system();
}

void test_sub_job(void* data)
{
	// Do some shit
	int apa = 1;
	int palle = 2;

}

void test_job(void* params)
{
	
	const unsigned n_jobs = 200;
	JobDeclaration jobs[n_jobs];
	size_t ets_stack_size = sizeof(jobs);

	for(unsigned i = 0; i < n_jobs; ++i)
	{
		jobs[i] = JobDeclaration(&test_sub_job, NULL);
	}

	Counter* counter = NULL;
	schedule_jobs((JobDeclaration*)&jobs, n_jobs, &counter);
	wait_for_counter(counter);
}


DWORD WINAPI thread_entry_job(void* params)
{
	Fiber* this_fiber = convert_thread_to_fiber(params);

	JobDeclaration job = JobDeclaration(&test_job, NULL);
	Counter* counter = NULL;
	schedule_jobs(&job, 1, &counter);
	
	global_counter2--;

	while(global_counter2.load() > 0) { ; }
	
	wait_for_counter(counter);


	ConvertFiberToThread();
	global_counter--;
	return 0;
}


int Application::run()
{

	
	// Queue up system jobs
	//_scheduler.enqueue(&thread_entry_job, PRIORITY_NORMAL);
	//_scheduler.enqueue(&thread_entry_job, PRIORITY_NORMAL);
	//_scheduler.enqueue(&game_update, HIGH_PRIORITY);
	//_scheduler.enqueue(&render, HIGH_PRIORITY);

	global_counter.store(4);
	global_counter2.store(4);

	create_worker_thread(thread_entry_job, 1<<0);
	create_worker_thread(thread_entry_job, 1<<1);
	create_worker_thread(thread_entry_job, 1<<2);
	create_worker_thread(thread_entry_job, 1<<3);
	//create_worker_thread(thread_entry_job, 1<<4);
	//create_worker_thread(thread_entry_job, 1<<5);


	
	// Wait for all work to be done
	//wait_for_signal(&_signal);

	// Clean up

	while(global_counter.load() != 0)
	{
		Sleep(1);
	}


return 1;
}

}