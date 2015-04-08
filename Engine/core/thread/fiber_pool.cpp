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
}


void initialize_fiber_pool(FiberPool* fiber_pool)
{
	fiber_pool->small_fibers = malloc(sizeof(void*) * fiber_pool->small_fiber_count);
	fiber_pool->large_fibers = malloc(sizeof(void*) * fiber_pool->large_fiber_count);
	for(unsigned i = 0; i < fiber_pool->small_fiber_count; ++i)
	{
		void* fiber = (void*)((size_t)fiber_pool->small_fibers + sizeof(void*) * i);
		fiber = CreateFiber(SMALL_FIBER_STACK_SIZE, &internal_fiber_proc, 0);
	}
		
	for (unsigned i = 0; i < fiber_pool->large_fiber_count; ++i)
	{
		void* fiber = (void*)((size_t)fiber_pool->large_fibers + sizeof(void*)* i);
		fiber = CreateFiber(LARGE_FIBER_STACK_SIZE, &internal_fiber_proc, 0);
	}

}

	
}