#include "d3d12_heap_allocator.h"
#include "d3d12_best_fit_allocator.h"
#include <math/math.h>
#include <core/assert/assert.h>

namespace ue
{


D3D12HeapAllocator::D3D12HeapAllocator(ID3D12Device* device, const D3D12HeapAllocatorDesc & desc)
	: _device(device)
	, _desc(desc)
{
	_best_fit_allocator = new D3D12BestFitAllocator(this);
}

D3D12HeapAllocator::~D3D12HeapAllocator()
{
	delete _best_fit_allocator;
}

ID3D12Heap* D3D12HeapAllocator::get_heap(const D3D12Allocation& allocation)
{
	return _heaps[allocation.heap].Get();
}

D3D12_GPU_VIRTUAL_ADDRESS D3D12HeapAllocator::get_offset(const D3D12Allocation& allocation)
{
	return (D3D12_GPU_VIRTUAL_ADDRESS)(allocation.offset * _desc.slot_size);
}

D3D12Allocation D3D12HeapAllocator::allocate(unsigned size_in_bytes, unsigned align)
{
	unsigned slot_count = Math::max(size_in_bytes / _desc.slot_size, 1U);
	D3D12Allocation allocation;
	allocation = _best_fit_allocator->allocate(slot_count);
	allocation.allocator_type = _desc.allocator_type;
	return allocation;
}

void D3D12HeapAllocator::deallocate(const D3D12Allocation& allocation)
{
	_best_fit_allocator->deallocate(allocation);
}

D3D12HeapHandle D3D12HeapAllocator::create_heap()
{
	D3D12_HEAP_DESC desc = {};
	desc.Alignment = 0;
	desc.SizeInBytes = _desc.size;
	desc.Properties = CD3DX12_HEAP_PROPERTIES(_desc.type);
	desc.Flags = _desc.flags;

	ComPtr<ID3D12Heap> heap;
	HRESULT hr = _device->CreateHeap(&desc, IID_PPV_ARGS(&heap));
	UENSURE(SUCCEEDED(hr));

	D3D12HeapHandle heap_handle = (D3D12HeapHandle)_heaps.size();
	_heaps.push_back(heap);
	return heap_handle;
}

void D3D12HeapAllocator::destroy_heap(D3D12HeapHandle handle)
{
	// Impl?
}

unsigned D3D12HeapAllocator::slot_count()
{
	return _desc.size / _desc.slot_size;
}


}