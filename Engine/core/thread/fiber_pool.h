#pragma once

#include <atomic>
#include "threading.h"

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
		fiber_lock(&_lock);
	}

	inline void unlock()
	{
		fiber_unlock(&_lock);
	}
private:
	std::atomic<bool> _lock;
	unsigned _fiber_count[2];
	Fiber* _fibers[2];
};


}