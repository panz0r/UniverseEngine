#pragma once
#include <d3d12.h>
#include <vector>
#include "../com/com_ptr.h"

namespace ue
{

struct D3D12DescriptorHandle
{
	unsigned index;
	unsigned count;
};

class D3D12OfflineDescriptorHeap
{
public:
	D3D12OfflineDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, unsigned heap_size);
	~D3D12OfflineDescriptorHeap();

	D3D12DescriptorHandle allocate(unsigned count);
	void release(D3D12DescriptorHandle);

	D3D12_CPU_DESCRIPTOR_HANDLE cpu_descriptor_handle(D3D12DescriptorHandle handle);

private:
	struct Entry
	{
		unsigned heap;
		unsigned heap_offset;
		unsigned count;

		unsigned index;
		unsigned next;
		unsigned prev;

		inline unsigned begin() { return heap_offset; }
		inline unsigned end() { return heap_offset + count; }
		inline unsigned size() { return count; }
	};

	D3D12DescriptorHandle make_handle(Entry* entry, unsigned count);

	bool find_free_range(unsigned count, Entry*& entry);
	void try_merge_with_next(Entry* entry);
	void allocate_from_entry(Entry* entry, unsigned count);
	void insert_before(unsigned heap, unsigned begin, unsigned count, Entry* entry);
	void insert_last(unsigned heap, unsigned begin, unsigned count);


	Entry* create_heap();
	Entry* free_begin();
	Entry* free_last();
	Entry* create_entry(unsigned heap, unsigned begin, unsigned count);
	Entry* next_entry(Entry* entry);
	Entry* prev_entry(Entry* entry);
	void detach(Entry* entry);

private:
	enum { INVALID_ENTRY = 0xffffffff };

	struct Heap
	{
		ComPtr<ID3D12DescriptorHeap> heap;
	};

	ID3D12Device* _device;
	D3D12_DESCRIPTOR_HEAP_TYPE _type;
	unsigned _heap_size;
	unsigned _descriptor_increment_size;

	unsigned _free_list_begin;
	unsigned _free_list_end;
	std::vector<Entry> _entries;
	std::vector<unsigned> _free_entries;
	std::vector<Heap> _heaps;

};

}