
#include "atomic_spin_lock.h"

namespace em
{

class Counter
{
public:
	Counter() 
	{
		_counter.store(-1, std::memory_order_seq_cst);
	}

	Counter(int count)
	{
		_counter.store(count, std::memory_order_seq_cst);
	}

	inline void decrease()
	{
		_counter.fetch_sub(1, std::memory_order_seq_cst);
	}

	inline bool is_zero()
	{
		int val = 0;
		return _counter.compare_exchange_weak(val, -1, std::memory_order_acquire);
	}

	inline bool aquire(int count)
	{
		int val = -1;
		return _counter.compare_exchange_weak(val, count, std::memory_order_acquire);
	}

private:
	std::atomic<int> _counter;
};

// temporary counter pool
class CounterPool
{
public:
	CounterPool(unsigned count)
		: _counter_count(count)
	{
		_counters = new Counter[count];
	}

	Counter* get_counter(int count)
	{
		for(unsigned i = 0; i < _counter_count; ++i)
		{
			if(_counters[i].aquire(count))
				return &_counters[i];
		}
		assert(false && "out of counter");
		return NULL;
	}


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
	unsigned _counter_count;
	
	Counter* _counters;
};

}