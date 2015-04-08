#include "wait_list.h"

namespace em
{

WaitList::WaitList(unsigned backlog_size)
: _backlog_size(backlog_size)
{
	_waiting_fibers = new WaitList[backlog_size];
	memset(_waiting_fibers, 0, sizeof(WaitEntry)*backlog_size);
}

WaitList::~WaitList()
{
}


unsigned WaitList::insert(const WaitEntry& entry)
{
	unsigned index = _current_index.fetch_add(1, std::memory_order_relaxed);
	if(index == _backlog_size) {
		_current_index.exchange(1, std::memory_order_relaxed);
		index = 0;
	}
}

}