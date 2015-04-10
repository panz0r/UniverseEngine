#pragma once

namespace em
{

struct Fiber;

class FiberPool
{
public:
	FiberPool(unsigned small_fiber_count, unsigned large_fiber_count);
	~FiberPool();
	
private:
	
	unsigned _small_fiber_count;
	unsigned _large_fiber_count;
	Fiber* _small_fibers;
	Fiber* _large_fibers;
};


}