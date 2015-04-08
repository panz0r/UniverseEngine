#pragma once

#include <atomic>

namespace em
{

struct Fiber;

struct WaitEntry
{
	Fiber* waiting_fiber;
	std::atomic<int> wait_counter;
};

class WaitList
{
public:
	WaitList(unsigned backlog_size);
	~WaitList();

	unsigned insert(const WaitEntry& entry);

private:
	unsigned _backlog_size;
	std::atomic<unsigned> _current_index;
	WaitEntry* _waiting_fibers;

};

}