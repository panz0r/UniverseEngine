#pragma once

#include <atomic>

namespace em
{

class Counter;

__declspec(align(64)) struct WaitEntry
{
	void* fiber_handle;
	Counter* counter;

	//char padding[64 - sizeof(void*)*2];
};

class WaitList
{
public:
	WaitList(unsigned backlog_size);
	~WaitList();

	void insert(Counter* counter);
	WaitEntry* get_next_ready_fiber(); // TODO, priority

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

	unsigned _backlog_size;
	std::atomic<bool> _lock;
	WaitEntry* _waiting_fibers;

};

}