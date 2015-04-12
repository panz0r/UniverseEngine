#pragma once

#include <atomic>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace em
{

typedef LPTHREAD_START_ROUTINE ThreadFunc;
typedef DWORD_PTR affinity_t;

struct Fiber;

Fiber* convert_thread_to_fiber(void* params);

void create_worker_thread(ThreadFunc func, affinity_t affinity_mask);

inline void fiber_lock(std::atomic<bool>* lock)
{
	bool exchange = false;
	while (!lock->compare_exchange_weak(exchange, true, std::memory_order_acquire))
	{
		exchange = false;
	}
	assert(exchange == false && "something fucked up");

}

inline void fiber_unlock(std::atomic<bool>* lock)
{
	bool val = lock->load(std::memory_order_seq_cst);
	assert(val == true && "unlocking unlocked lock");
	lock->store(false, std::memory_order_release);
}

}