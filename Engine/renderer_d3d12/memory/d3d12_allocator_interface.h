#pragma once

#include "d3d12_allocation.h"

namespace ue
{

class D3D12HeapAllocatorInterface
{
public:
	virtual D3D12Allocation allocate(unsigned size, unsigned align = 0U) = 0;
	virtual void deallocate(const D3D12Allocation& allocation) = 0;

	virtual D3D12HeapHandle create_heap() = 0;
	virtual void destroy_heap(D3D12HeapHandle handle) = 0;
	virtual unsigned slot_count() = 0;
};


class D3D12AllocatorInterface
{
public:
	virtual D3D12Allocation allocate(unsigned size, unsigned align = 0U) = 0;
	virtual void deallocate(const D3D12Allocation& allocation) = 0;
};

}