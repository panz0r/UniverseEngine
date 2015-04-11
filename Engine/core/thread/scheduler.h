#pragma once

namespace em
{

enum { PRIORITY_HIGH, PRIORITY_NORMAL, PRIORITY_LOW };

struct FiberPool;
struct JobDeclaration;
class Counter;
class JobQueue;

class Scheduler
{
public:
	Scheduler(FiberPool* fiber_pool);
	~Scheduler() {}

	void schedule_jobs(JobDeclaration** job_decls, unsigned count, Counter* counter, unsigned priortiy);

private:

	FiberPool* _fiber_pool;
	JobQueue* _job_queues[3];


};

void create_scheduler(FiberPool* fiber_pool);
void schedule_jobs(JobDeclaration** jobs, unsigned count, Counter** counter, unsigned priority = PRIORITY_NORMAL);


}