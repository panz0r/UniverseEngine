#pragma once
#include <vector>

namespace ue
{

inline void *ptr_offset(void *ptr, size_t offset)
{
	return reinterpret_cast<void*>(reinterpret_cast<size_t>(ptr) + offset);
}

inline size_t range_size(void *begin, void *end) {
	return reinterpret_cast<size_t>(end) - reinterpret_cast<size_t>(begin);
}

inline void *ptr_align(void *ptr, unsigned align) {
	return reinterpret_cast<void*>((reinterpret_cast<size_t>(ptr) + (align - 1)) & ~(align - 1));
}

inline void *ptr_subtract(void *ptr1, void *ptr2) {
	return reinterpret_cast<void*>(reinterpret_cast<size_t>(ptr1), reinterpret_cast<size_t>(ptr2));
}


class FirstFitAllocator
{
public:
	FirstFitAllocator(void *heap_begin, size_t heap_size);
	void *allocate(size_t size, unsigned align);

private:
	struct Allocation
	{
		void *begin;
		void *end;
		unsigned next_free;
	};
	
	void *_begin;
	void *_current;
	void *_end;
	unsigned _first_free;
	std::vector<Allocation> _free_list;
	std::vector<Allocation> _allocated;
};

}