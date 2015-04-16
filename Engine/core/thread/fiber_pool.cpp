#include "fiber_pool.h"
#include "fiber_system.h"

//////////////////////////////////////////////////////////////////////////

#include <malloc.h>

#define SMALL_FIBER_STACK_SIZE 1024*1024
#define LARGE_FIBER_STACK_SIZE 512*1024

namespace ue
{

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
		_fibers[SMALL_STACK][i].fiber_handle = create_fiber(SMALL_FIBER_STACK_SIZE, &internal::fiber_proc, &_fibers[SMALL_STACK][i]);
	}

	for (unsigned i = 0; i < large_fiber_count; ++i)
	{
		_fibers[LARGE_STACK][i].fiber_handle = CreateFiber(SMALL_FIBER_STACK_SIZE, &internal::fiber_proc, &_fibers[LARGE_STACK][i]);
	}
}

FiberPool::~FiberPool()
{
}

Fiber* FiberPool::get_fiber(unsigned type)
{
	for(unsigned i = 0; i < _fiber_count[type]; ++i) {
		if(_fibers[type][i].in_use == false) {
			return &_fibers[type][i];
		}
	}

	assert(false && "out of fibers");
	return NULL;
}

}