#pragma once

#include <atomic>

namespace ue
{


struct spin_lock_t
{
	spin_lock_t() 
	{
		_locked.clear();
	}
	std::atomic_flag _locked;
};

inline void atomic_lock(spin_lock_t& lock)
{
	
	while(lock._locked.test_and_set(std::memory_order_acquire)) { ; }
}

inline void atomic_unlock(spin_lock_t& lock)
{
	lock._locked.clear(std::memory_order_release);
}


}