#pragma once

#include <atomic>
#include "threading.h"

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

	inline void lock()
	{
		fiber_lock(&_lock);
	}

	inline void unlock()
	{
		fiber_unlock(&_lock);
	}

private:

	unsigned _backlog_size;
	std::atomic<bool> _lock;
	WaitEntry* _waiting_fibers;

};

}