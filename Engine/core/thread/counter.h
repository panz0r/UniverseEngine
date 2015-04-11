
#include <atomic>

namespace em
{

class Counter
{
public:
	Counter(int count)
	{
		_counter.store(count, std::memory_order_acquire);
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

private:
	std::atomic<int> _counter;
};

}