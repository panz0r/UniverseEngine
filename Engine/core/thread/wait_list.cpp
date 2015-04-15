#include "wait_list.h"
#include <malloc.h>
#include "counter.h"


#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace em
{

WaitList::WaitList(unsigned backlog_size)
: _backlog_size(backlog_size)
{
	size_t size = sizeof(WaitEntry);
	_waiting_fibers = (WaitEntry*)_aligned_malloc(size*backlog_size, 64);
	memset(_waiting_fibers, 0, size*backlog_size);
}

WaitList::~WaitList()
{
}

void WaitList::insert(Counter* counter, void* fiber_handle)
{
	for(unsigned i = 0; i < _backlog_size; ++i)
	{
		if(_waiting_fibers[i].counter == NULL) {
			_waiting_fibers[i].counter = counter;
			_waiting_fibers[i].fiber_handle = fiber_handle;
			return;
		}
	}

	assert(false && "wait list is full");
}

WaitEntry* WaitList::get_ready_fiber()
{
	for (unsigned i = 0; i < _backlog_size; ++i)
	{
		if(_waiting_fibers[i].counter != NULL && _waiting_fibers[i].counter->is_zero())
		{
			return &_waiting_fibers[i];
		}
	}
	return NULL;
}

}