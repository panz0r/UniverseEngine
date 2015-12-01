#include "d3d12_best_fit_allocator.h"

namespace ue
{


D3D12BestFitAllocator::D3D12BestFitAllocator(D3D12HeapAllocatorInterface* heap_allocator)
: _allocator(heap_allocator)
, _free_list_begin(INVALID_ENTRY)
, _free_list_end(INVALID_ENTRY)
{
}

D3D12BestFitAllocator::~D3D12BestFitAllocator()
{
}

D3D12Allocation D3D12BestFitAllocator::allocate(unsigned slot_count)
{
	Entry* entry;
	if (find_best_fit(slot_count, entry)) {
		return make_allocation(entry, slot_count);
	}

	D3D12HeapHandle heap = _allocator->create_heap();
	insert_last(heap, 0, _allocator->slot_count());
	entry = free_list_end();
	return make_allocation(entry, slot_count);
}

void D3D12BestFitAllocator::deallocate(const D3D12Allocation& allocation)
{
	Entry* entry = free_list_begin();
	while (entry)
	{
		if (entry->heap != allocation.heap) {
			entry = next_entry(entry);
			continue;
		}

		if (entry->end() == allocation.offset) {
			entry->count += allocation.count;
			try_merge_with_next(entry);
			return;
		}
		else if (allocation.offset + allocation.count == entry->begin()) {
			entry->count += allocation.count;
			entry->offset -= allocation.count;
			return;
		}
		else if (allocation.offset + allocation.count < entry->begin()) {
			insert_before(allocation.heap, allocation.offset, allocation.count, entry);
			return;
		}
		entry = next_entry(entry);
	}

	insert_last(allocation.heap, allocation.offset, allocation.count);
}

D3D12Allocation D3D12BestFitAllocator::make_allocation(Entry* entry, unsigned count)
{
	D3D12Allocation allocation = { entry->heap, entry->offset, count };
	allocate_from_entry(entry, count);
	return allocation;
}


void D3D12BestFitAllocator::allocate_from_entry(Entry* entry, unsigned count)
{
	if (entry->count == count) {
		detach(entry);
	}
	else {
		entry->offset += count;
		entry->count -= count;
	}
}


bool D3D12BestFitAllocator::find_best_fit(unsigned count, Entry*& entry)
{
	unsigned best_count = UINT_MAX;
	Entry* iter = free_list_begin();
	while (iter) {
		if (iter->count >= count && iter->count < best_count) {
			best_count = iter->count;
			entry = iter;
		}
		iter = next_entry(iter);
	}
	return (best_count != UINT_MAX);
}

void D3D12BestFitAllocator::insert_last(D3D12HeapHandle heap, unsigned offset, unsigned count)
{
	Entry *new_entry = create_entry(heap, offset, count);
	new_entry->next = INVALID_ENTRY;
	new_entry->prev = _free_list_end;
	Entry* prev = prev_entry(new_entry);
	if (prev)
		prev->next = new_entry->index;

	_free_list_end = new_entry->index;

	if (_free_list_begin == INVALID_ENTRY)
		_free_list_begin = new_entry->index;
}

void D3D12BestFitAllocator::insert_before(D3D12HeapHandle heap, unsigned offset, unsigned count, Entry* entry)
{
	Entry *new_entry = create_entry(heap, offset, count);
	new_entry->prev = entry->prev;
	new_entry->next = entry->index;
	Entry* prev = prev_entry(entry);
	if (prev)
		prev->next = new_entry->index;

	entry->prev = new_entry->index;

	if (_free_list_begin == entry->index)
		_free_list_begin = new_entry->index;
}

void D3D12BestFitAllocator::try_merge_with_next(Entry* entry)
{
	Entry* next = next_entry(entry);
	if (next) {
		if (entry->heap == next->heap && entry->end() == next->begin()) {
			entry->count += next->count;
			detach(next);
		}
	}
}


D3D12BestFitAllocator::Entry* D3D12BestFitAllocator::create_entry(D3D12HeapHandle heap, unsigned offset, unsigned count)
{
	Entry* new_entry;
	if (_free_entries.size() > 0) {
		unsigned entry_index = _free_entries.back();
		_free_entries.pop_back();
		new_entry = &_entries[entry_index];
		new_entry->heap = heap;
		new_entry->offset = offset;
		new_entry->count = count;
	}
	else
	{
		Entry _new_entry = { heap, offset, count, (unsigned)_entries.size(), INVALID_ENTRY, INVALID_ENTRY };
		_entries.push_back(_new_entry);
		new_entry = &_entries.back();
	}
	return new_entry;
}


D3D12BestFitAllocator::Entry* D3D12BestFitAllocator::next_entry(Entry* entry)
{
	if (entry->next == INVALID_ENTRY)
		return nullptr;
	return &_entries[entry->next];
}

D3D12BestFitAllocator::Entry* D3D12BestFitAllocator::prev_entry(Entry* entry)
{
	if (entry->prev == INVALID_ENTRY)
		return nullptr;
	return &_entries[entry->prev];
}


D3D12BestFitAllocator::Entry* D3D12BestFitAllocator::free_list_end()
{
	if (_free_list_end == INVALID_ENTRY)
		return nullptr;
	return &_entries[_free_list_end];
}

D3D12BestFitAllocator::Entry* D3D12BestFitAllocator::free_list_begin()
{
	if (_free_list_begin == INVALID_ENTRY)
		return nullptr;
	return &_entries[_free_list_begin];
}

void D3D12BestFitAllocator::detach(Entry * entry)
{
	Entry* next = next_entry(entry);
	Entry* prev = prev_entry(entry);
	if (prev)
		prev->next = entry->next;
	if (next)
		next->prev = entry->prev;
	if (entry->index == _free_list_begin)
		_free_list_begin = entry->next;
	if (entry->index == _free_list_end)
		_free_list_end = entry->prev;

	entry->next = INVALID_ENTRY;
	entry->prev = INVALID_ENTRY;
	_free_entries.push_back(entry->index);

}


}