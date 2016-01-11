#pragma once

namespace ue
{

typedef unsigned D3D12HeapHandle;
typedef unsigned long long D3D12HeapOffset;

struct D3D12Allocation
{
	D3D12HeapHandle heap;	// holds the index to the heap
	unsigned offset;		// the slot offset into the heap
	unsigned count;			// the slot count on the heap
	unsigned allocator_type;	// allocator information
};

// Todo, remove?
typedef D3D12Allocation D3D12DescriptorAllocation;


}