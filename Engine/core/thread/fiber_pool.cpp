#include "fiber_pool.h"

//////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <malloc.h>

#define SMALL_FIBER_STACK_SIZE 64*1024
#define LARGE_FIBER_STACK_SIZE 512*1024

namespace em
{

void CALLBACK internal_fiber_proc(void* fiber_params)
{
	Fiber* fiber = (Fiber*)fiber_params;
	fiber->func();
}


FiberPool::FiberPool(unsigned small_fiber_count, unsigned large_fiber_count)
: _small_fiber_count(small_fiber_count)
, _large_fiber_count(large_fiber_count)
{
	_small_fibers = new Fiber[small_fiber_count];
	_large_fibers = new Fiber[large_fiber_count];
	memset(_small_fibers, 0, sizeof(Fiber)*small_fiber_count);
	memset(_large_fibers, 0, sizeof(Fiber)*large_fiber_count);

	for(unsigned i = 0; i < small_fiber_count; ++i)
	{
		_small_fibers[i].fiber_handle = CreateFiber(SMALL_FIBER_STACK_SIZE, &internal_fiber_proc, &_small_fibers[i]);
	}

	for (unsigned i = 0; i < large_fiber_count; ++i)
	{
		_large_fibers[i].fiber_handle = CreateFiber(LARGE_FIBER_STACK_SIZE, &internal_fiber_proc, &_large_fibers[i]);
	}
}

FiberPool::~FiberPool()
{
}
	
}