#include "fiber_pool.h"
#include "fiber.h"
#include "fiber_system.h"

//////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <malloc.h>

#define SMALL_FIBER_STACK_SIZE 1024*1024
#define LARGE_FIBER_STACK_SIZE 512*1024

namespace em
{

void CALLBACK internal_fiber_proc(void* fiber_params)
{
	Fiber* fiber = (Fiber*)fiber_params;
	while(true) {
		HANDLE current_fiber = GetCurrentFiber();
		//if(fiber->func == NULL)
		//{
		//	char tmp_buffer[128];
		//	sprintf(tmp_buffer, "(%d): fiber: %08x, handle: %08x, cooldown: %d\n", GetCurrentThreadId(), GetCurrentFiber(), fiber->fiber_handle, fiber->cooldown);
		//	OutputDebugString(tmp_buffer);
		//}
		assert(fiber->func != NULL && "invalid fiber function! ");
		fiber->func(fiber->params);

		// fiber func is done here
		release_fiber(fiber);
		switch_fiber();
		assert(fiber->func != NULL && "invalid fiber function! ");
	}
}


FiberPool::FiberPool(unsigned small_fiber_count, unsigned large_fiber_count)
{
	_fiber_count[SMALL_STACK] = small_fiber_count;
	_fiber_count[LARGE_STACK] = small_fiber_count;
	_fibers[SMALL_STACK] = new Fiber[small_fiber_count];
	_fibers[LARGE_STACK] = new Fiber[small_fiber_count];
	memset(_fibers[SMALL_STACK], 0, sizeof(Fiber)*small_fiber_count);
	memset(_fibers[LARGE_STACK], 0, sizeof(Fiber)*large_fiber_count);

	for(unsigned i = 0; i < small_fiber_count; ++i)
	{
		_fibers[SMALL_STACK][i].fiber_handle = CreateFiber(SMALL_FIBER_STACK_SIZE, &internal_fiber_proc, &_fibers[SMALL_STACK][i]);
	}

	for (unsigned i = 0; i < large_fiber_count; ++i)
	{
		_fibers[LARGE_STACK][i].fiber_handle = CreateFiber(SMALL_FIBER_STACK_SIZE, &internal_fiber_proc, &_fibers[LARGE_STACK][i]);
	}
}

FiberPool::~FiberPool()
{
}

Fiber* FiberPool::get_fiber(unsigned type)
{
	for(unsigned i = 0; i < _fiber_count[type]; ++i) {
		if(_fibers[type][i].func == NULL && --_fibers[type][i].cooldown <= 0) {
			return &_fibers[type][i];
		}
	}

	assert(false && "out of fibers");
	return NULL;
}

}