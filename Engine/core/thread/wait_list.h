#pragma once

#include <atomic>
#include "atomic_spin_lock.h"

namespace em
{

class Counter;

/*__declspec(align(64)) */struct WaitEntry
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

	void insert(Counter* counter, void* fiber_handle);
	WaitEntry* get_ready_fiber(); // TODO, priority

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

	unsigned _backlog_size;
	WaitEntry* _waiting_fibers;

};

}