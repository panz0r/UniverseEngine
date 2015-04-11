#include "scheduler.h"
#include "job_queue.h"

namespace em
{

Scheduler::Scheduler(FiberPool* fiber_pool)
 : _fiber_pool(fiber_pool)
{
	unsigned max_jobs = 100;
	_job_queues[PRIORITY_HIGH] = new JobQueue(max_jobs);
	_job_queues[PRIORITY_NORMAL] = new JobQueue(max_jobs);
	_job_queues[PRIORITY_LOW] = new JobQueue(max_jobs);
}

void Scheduler::schedule_jobs(JobDeclaration** job_decls, unsigned count, Counter* counter, unsigned priortiy)
{
	_job_queues[priortiy]->lock();
	for(unsigned i = 0; i < count; ++i) {
		_job_queues[priortiy]->enqueue(job_decls[i]);
	}
	_job_queues[priortiy]->unlock();
}


namespace internal
{
	Scheduler *g_scheduler = NULL;
}

void create_scheduler(FiberPool* fiber_pool)
{
	internal::g_scheduler = new Scheduler(fiber_pool);
}

void schedule_jobs(JobDeclaration** jobs, unsigned count, Counter** counter, unsigned priority)
{
	*counter = create_counter();
	internal::g_scheduler->schedule_jobs(jobs, count, *counter, priority);
}

}