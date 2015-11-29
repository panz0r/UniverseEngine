#pragma once

#include "d3d12_allocator_interface.h"
#include <vector>

namespace ue
{

#define INVALID_ENTRY 0xffffffff

class D3D12BestFitAllocator
{
public:
	D3D12BestFitAllocator(D3D12HeapAllocatorInterface* heap_allocator);
	~D3D12BestFitAllocator();

	D3D12Allocation allocate(unsigned slot_count);
	void deallocate(D3D12Allocation allocation);

private:
	struct Entry
	{
		D3D12HeapHandle heap;
		unsigned offset;
		unsigned count;

		unsigned index;
		unsigned next;
		unsigned prev;

		inline unsigned begin() { return offset; }
		inline unsigned end() { return offset + count; }
		inline unsigned size() { return count; }
	};

	D3D12Allocation make_allocation(Entry* entry, unsigned count);
	void allocate_from_entry(Entry* entry, unsigned count);
	bool find_best_fit(unsigned count, Entry*& entry);
	void insert_last(D3D12HeapHandle heap, unsigned offset, unsigned count);
	void insert_before(D3D12HeapHandle heap, unsigned offset, unsigned count, Entry* entry);
	void try_merge_with_next(Entry* entry);

	Entry* create_entry(D3D12HeapHandle heap, unsigned offset, unsigned count);
	Entry* next_entry(Entry* entry);
	Entry* prev_entry(Entry* entry);
	Entry* free_list_end();
	Entry* free_list_begin();
	void detach(Entry* entry);

private:

	D3D12HeapAllocatorInterface* _allocator;
	unsigned _free_list_begin;
	unsigned _free_list_end;
	std::vector<Entry> _entries;
	std::vector<unsigned> _free_entries;
};


}