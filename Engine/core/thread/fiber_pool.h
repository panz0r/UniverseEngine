#pragma once

#include <atomic>
#include "atomic_spin_lock.h"

namespace em
{

struct Fiber;

enum { SMALL_STACK, LARGE_STACK };

class FiberPool
{
public:
	FiberPool(unsigned small_fiber_count, unsigned large_fiber_count);
	~FiberPool();


	Fiber* get_fiber(unsigned type);
	
	inline void lock()
	{
		atomic_lock(_lock);
	}

	inline void unlock()
	{
		atomic_unlock(_lock);
	}
private:
	spin_lock_t _lock;

	unsigned _fiber_count[2];
	Fiber* _fibers[2];
};


}