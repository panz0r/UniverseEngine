#include "application.h"

#include <core/thread/thread.h>
#include <core/thread/fiber_system.h>
#include <core/thread/job_declaration.h>
#include <core/thread/counter.h>

namespace ue
{

Application::Application()
{}

Application::~Application()
{}

void Application::initialize()
{
	initialize_fiber_system();
}


// Make sure some time is spent here
#pragma optimize("", off)
void test_sub_job(void* data)
{
	// Do some shit
	int apa = 0;
	for(int i = 0; i < 1000000; ++i)
	{
		apa++;
	}
	
	//int apa = 1;
	//int palle = apa;
	//Sleep(rand()%3);
}

void test_sub_job_2(void* data)
{
	// Do some shit
	int apa = 0;
	for (int i = 0; i < 2000000; ++i)
	{
		apa++;
	}

	//int apa = 1;
	//int palle = apa;
	//Sleep(rand()%3);
}

#pragma optimize("", on)


void test_job(void* params)
{
	
	const unsigned n_jobs = 4900;
	JobDeclaration jobs[n_jobs];
	size_t ets_stack_size = sizeof(jobs);
	int arne = 0;
	for(unsigned i = 0; i < n_jobs; ++i)
	{
		jobs[i] = JobDeclaration(rand()%10 > 8 ? &test_sub_job : &test_sub_job_2, &arne);
	}

	Counter* counter = NULL;
	schedule_jobs((JobDeclaration*)&jobs, n_jobs, &counter);
	wait_for_counter(counter);
}


DECLARE_THREAD worker_thread(void* params)
{
	convert_thread_to_fiber(params);

	while(true)
	{
		do_work();
	}

	convert_fiber_to_thread();
	return 0;
}


int Application::run()
{

	
	// Queue up system jobs
	//_scheduler.enqueue(&thread_entry_job, PRIORITY_NORMAL);
	//_scheduler.enqueue(&thread_entry_job, PRIORITY_NORMAL);
	//_scheduler.enqueue(&game_update, HIGH_PRIORITY);
	//_scheduler.enqueue(&render, HIGH_PRIORITY);

	create_worker_thread(worker_thread, 1<<0);
	create_worker_thread(worker_thread, 1<<1);
	create_worker_thread(worker_thread, 1<<2);
	create_worker_thread(worker_thread, 1<<3);
	create_worker_thread(worker_thread, 1<<4);
	create_worker_thread(worker_thread, 1<<5);
	create_worker_thread(worker_thread, 1<<6);
	create_worker_thread(worker_thread, 1<<7);
	
	//create_worker_thread(worker_thread, 1<<5);
	//create_worker_thread(worker_thread, 1<<6);

	JobDeclaration job = JobDeclaration(&test_job, NULL);
	Counter* counter = NULL;
	schedule_jobs(&job, 1, &counter);

	while(!counter->is_zero())
	{
		Sleep(1);
	}

return 1;
}

}