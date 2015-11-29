#pragma once

#define ALIGN(x, align) (((x) + (align-1)) & ~(align-1))

namespace ue
{

typedef unsigned D3D12HeapHandle;
typedef unsigned long long D3D12HeapOffset;

struct D3D12Allocation
{
	D3D12HeapHandle heap;
	unsigned offset;
	unsigned count;
};

class D3D12HeapAllocatorInterface
{
public:
	virtual D3D12HeapHandle create_heap() = 0;
	virtual void destroy_heap(D3D12HeapHandle handle) = 0;
	virtual unsigned slot_count() = 0;
};


}