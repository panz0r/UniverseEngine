#pragma once

namespace em
{

enum { PRIORITY_HIGH, PRIORITY_NORMAL, PRIORITY_LOW };

struct Fiber;
class FiberPool;
struct JobDeclaration;
class Counter;
class JobQueue;
class WaitList;

class Scheduler
{
public:
	Scheduler(FiberPool* fiber_pool);
	~Scheduler() {}

	void schedule_jobs(JobDeclaration* job_decls, int count, Counter* counter, unsigned priortiy);
	void add_to_wait_list_and_switch_fiber(Counter* counter);
	void release_fiber(Fiber* fiber);

private:
	void get_next_job(JobDeclaration** job_decl, Counter** counter);
	Fiber* get_fiber(JobDeclaration* job_decl, Counter* counter);

	Counter* create_counter(int count);

	FiberPool* _fiber_pool;
	WaitList* _wait_list;
	JobQueue* _job_queues[3];


};

void create_scheduler(FiberPool* fiber_pool);
void schedule_jobs(JobDeclaration* jobs, int count, Counter** counter, unsigned priority = PRIORITY_NORMAL);
void wait_for_counter(Counter* counter);
void release_fiber(Fiber* fiber);

}