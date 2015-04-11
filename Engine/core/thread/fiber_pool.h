#pragma once

#include <atomic>

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
	
	void lock()
	{
		bool val = false;
		while (!_lock.compare_exchange_weak(val, true, std::memory_order_acquire));
	}

	void unlock()
	{
		_lock.store(false, std::memory_order_release);
	}

private:
	std::atomic<bool> _lock;
	unsigned _fiber_count[2];
	Fiber* _fibers[2];
};


}