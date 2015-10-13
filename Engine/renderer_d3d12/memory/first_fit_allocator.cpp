#include "first_fit_allocator.h"

namespace ue
{

FirstFitAllocator::FirstFitAllocator(void *heap_begin, size_t heap_size)
: _begin(heap_begin)
, _current(heap_begin)
, _end(ptr_offset(heap_begin, heap_size))
{
}


void *FirstFitAllocator::allocate(size_t size, unsigned align)
{
	Allocation *free = &_free_list[_first_free];
	while (free) {
		void *begin = free->begin;
		void *end = free->end;
		void *aligned_begin(ptr_align(begin, align));
		size_t aligned_size = range_size(aligned_begin, end);
		if (size <= aligned_size) {
		}

		free = &_free_list[free->next_free];
	}
	return nullptr;
}


}