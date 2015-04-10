#pragma once

namespace em
{

struct FiberPool;
class JobQueue;

class Scheduler
{
public:
	enum { HIGH_PRIORITY, NORMAL_PRIORITY, LOW_PRIORITY, QUEUE_COUNT };
	Scheduler(FiberPool* fiber_pool) : _fiber_pool(fiber_pool) {}
	~Scheduler() {}

private:

	FiberPool* _fiber_pool;
	JobQueue* _job_queues[QUEUE_COUNT];


};


}