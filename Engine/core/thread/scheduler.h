#pragma once

namespace em
{

struct FiberPool;

class Scheduler
{
public:
	Scheduler(FiberPool* fiber_pool) : _fiber_pool(fiber_pool) {}
	~Scheduler() {}

private:

	FiberPool* _fiber_pool;

};


}