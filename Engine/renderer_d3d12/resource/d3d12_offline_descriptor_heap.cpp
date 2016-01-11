#include "d3d12_offline_descriptor_heap.h"
#include <renderer_d3d12\stdinclude.h>


namespace ue
{

D3D12OfflineDescriptorHeap::D3D12OfflineDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, unsigned heap_size)
	: _device(device)
	, _type(type)
	, _heap_size(heap_size)
	, _free_list_begin(INVALID_ENTRY)
	, _free_list_end(INVALID_ENTRY)
{
	_descriptor_increment_size = device->GetDescriptorHandleIncrementSize(type);
}

D3D12OfflineDescriptorHeap::~D3D12OfflineDescriptorHeap()
{
}

////////////////////////////////////////////////

D3D12DescriptorHandle D3D12OfflineDescriptorHeap::allocate(unsigned count)
{
	Entry* entry;
	if (find_free_range(count, entry)) {
		D3D12DescriptorHandle handle = make_handle(entry, count);
		allocate_from_entry(entry, count);
		return handle;
	}

	entry = create_heap();

	D3D12DescriptorHandle handle = make_handle(entry, count);
	allocate_from_entry(entry, count);
	return handle;
}

////////////////////////////////////////////////

void D3D12OfflineDescriptorHeap::release(D3D12DescriptorHandle handle)
{
	unsigned heap = handle.index / _heap_size;
	unsigned begin = handle.index % _heap_size;

	Entry* entry = free_begin();
	while (entry)
	{
		if (entry->heap != heap) {
			entry = next_entry(entry);
			continue;
		}

		if (entry->end() == begin) {
			entry->count += handle.count;
			try_merge_with_next(entry);
			return;
		} 
		else if (begin + handle.count == entry->begin()) {
			entry->count += handle.count;
			entry->heap_offset -= handle.count;
			return;
		}
		else if (begin + handle.count < entry->begin()) {
			insert_before(heap, begin, handle.count, entry);
			return;
		}
		entry = next_entry(entry);
	}

	insert_last(heap, begin, handle.count);
}

////////////////////////////////////////////////

D3D12_CPU_DESCRIPTOR_HANDLE D3D12OfflineDescriptorHeap::cpu_descriptor_handle(D3D12DescriptorHandle handle)
{
	unsigned heap_index = handle.index / _heap_size;
	unsigned offset = handle.index % _heap_size;
	Heap& heap = _heaps[heap_index];
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(heap.heap->GetCPUDescriptorHandleForHeapStart(), offset, _descriptor_increment_size);
}

////////////////////////////////////////////////

D3D12DescriptorHandle D3D12OfflineDescriptorHeap::make_handle(Entry* entry, unsigned count)
{
	D3D12DescriptorHandle handle;
	handle.index = entry->heap * _heap_size + entry->heap_offset;
	handle.count = count;
	return handle;
}

////////////////////////////////////////////////

bool D3D12OfflineDescriptorHeap::find_free_range(unsigned count, Entry*& entry)
{
	entry = free_begin();
	while (entry) {
		if (entry->count >= count)
			return true;
		entry = next_entry(entry);
	}
	return false;
}

////////////////////////////////////////////////

void D3D12OfflineDescriptorHeap::try_merge_with_next(Entry* entry)
{
	Entry* next = next_entry(entry);
	if (next) {
		if (entry->heap == next->heap && entry->end() == next->begin()) {
			entry->count += next->count;
			detach(next);
		}
	}
}

////////////////////////////////////////////////

void D3D12OfflineDescriptorHeap::allocate_from_entry(Entry * entry, unsigned count)
{
	if (entry->count == count) {
		detach(entry);
	}
	else {
		entry->heap_offset += count;
		entry->count -= count;
	}
}
	
////////////////////////////////////////////////

void D3D12OfflineDescriptorHeap::insert_before(unsigned heap, unsigned begin, unsigned count, Entry* entry)
{
	Entry* new_entry = create_entry(heap, begin, count);

	new_entry->prev = entry->prev;
	new_entry->next = entry->index;
	Entry* prev = prev_entry(entry);
	if (prev)
		prev->next = new_entry->index;
	entry->prev = new_entry->index;

	if (_free_list_begin == entry->index)
		_free_list_begin = new_entry->index;
}

////////////////////////////////////////////////

void D3D12OfflineDescriptorHeap::insert_last(unsigned heap, unsigned begin, unsigned count)
{
	Entry* new_entry = create_entry(heap, begin, count);

	new_entry->next = INVALID_ENTRY;
	new_entry->prev = _free_list_end;
	Entry* prev = prev_entry(new_entry);
	if (prev)
		prev->next = new_entry->index;

	_free_list_end = new_entry->index;

	if (_free_list_begin == INVALID_ENTRY)
		_free_list_begin = new_entry->index;
}

////////////////////////////////////////////////

D3D12OfflineDescriptorHeap::Entry* D3D12OfflineDescriptorHeap::create_heap()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = _type;
	desc.NodeMask = 0;
	desc.NumDescriptors = _heap_size;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	
	Heap heap = {};
	_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap.heap));

	unsigned heap_index = (unsigned)_heaps.size();
	_heaps.push_back(heap);
	insert_last(heap_index, 0, _heap_size);
	return free_last();
}

////////////////////////////////////////////////

D3D12OfflineDescriptorHeap::Entry* D3D12OfflineDescriptorHeap::free_begin()
{
	if(_free_list_begin == INVALID_ENTRY)
		return nullptr;
	return &_entries[_free_list_begin];
}

////////////////////////////////////////////////

D3D12OfflineDescriptorHeap::Entry* D3D12OfflineDescriptorHeap::free_last()
{
	if (_free_list_end == INVALID_ENTRY)
		return nullptr;
	return &_entries[_free_list_end];
}

////////////////////////////////////////////////

D3D12OfflineDescriptorHeap::Entry* D3D12OfflineDescriptorHeap::create_entry(unsigned heap, unsigned begin, unsigned count)
{
	Entry *new_entry;
	if (_free_entries.size() > 0) {
		unsigned index = _free_entries.back();
		_free_entries.pop_back();
		new_entry = &_entries[index];
		new_entry->heap = heap;
		new_entry->heap_offset = begin;
		new_entry->count = count;
	}
	else {
		Entry _entry = { heap, begin, count, (unsigned)_entries.size(), (unsigned)INVALID_ENTRY, (unsigned)INVALID_ENTRY };
		_entries.push_back(_entry);
		new_entry = &_entries.back();
	}
	return new_entry;
}

////////////////////////////////////////////////

D3D12OfflineDescriptorHeap::Entry* D3D12OfflineDescriptorHeap::next_entry(Entry* entry)
{
	if(entry->next == INVALID_ENTRY)
		return nullptr;
	return &_entries[entry->next];
}

////////////////////////////////////////////////

D3D12OfflineDescriptorHeap::Entry* D3D12OfflineDescriptorHeap::prev_entry(Entry* entry)
{
	if(entry->prev == INVALID_ENTRY)
		return nullptr;
	return &_entries[entry->prev];
}

////////////////////////////////////////////////

void D3D12OfflineDescriptorHeap::detach(Entry* entry)
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