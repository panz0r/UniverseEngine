#pragma once

#include <atomic>

namespace ue
{


struct AtomicSpinLock
{
	AtomicSpinLock() 
	{
		_locked.clear();
	}

	inline void lock() {
		while (_locked.test_and_set(std::memory_order_acquire)) {}
	}

	inline bool try_lock() {
		return !_locked.test_and_set(std::memory_order_acquire);
	}

	inline void unlock() {
		_locked.clear(std::memory_order_release);
	}

	std::atomic_flag _locked;
};


}