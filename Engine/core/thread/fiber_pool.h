#pragma once

namespace em
{

struct FiberPool
{
	unsigned small_fiber_count;
	unsigned large_fiber_count;
	void* small_fibers;
	void* large_fibers;
};


void initialize_fiber_pool(FiberPool* pool);


}