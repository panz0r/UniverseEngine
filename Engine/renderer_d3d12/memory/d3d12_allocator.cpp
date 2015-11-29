#include "d3d12_allocator.h"
#include "../renderer/d3dx12.h"

namespace ue
{

D3D12HeapAllocator::D3D12HeapAllocator(ID3D12Device* device, const D3D12AllocatorDesc & desc)
	: _device(device)
	, _desc(desc)
{
	_best_fit_allocator = new D3D12BestFitAllocator(this);
}

D3D12HeapAllocator::~D3D12HeapAllocator()
{
	delete _best_fit_allocator;
}

ID3D12Heap* D3D12HeapAllocator::get_heap(D3D12Allocation allocation)
{
	return _heaps[allocation.heap].Get();
}

D3D12_GPU_VIRTUAL_ADDRESS D3D12HeapAllocator::get_offset(D3D12Allocation allocation)
{
	return (D3D12_GPU_VIRTUAL_ADDRESS)(allocation.offset * _desc.slot_size);
}

D3D12Allocation D3D12HeapAllocator::allocate(unsigned size_in_bytes)
{
	unsigned aligned_size = ALIGN(size_in_bytes, _desc.alignment);
	unsigned slot_count = aligned_size / _desc.slot_size;
	return _best_fit_allocator->allocate(slot_count);
}

void D3D12HeapAllocator::deallocate(D3D12Allocation allocation)
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
	_device->CreateHeap(&desc, IID_PPV_ARGS(&heap));
	
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

////////////////////////////////////////////////////////////////////


D3D12ResourceAllocator::D3D12ResourceAllocator(ID3D12Device* device, const D3D12AllocatorDesc & desc)
	: _device(device)
	, _desc(desc)
{
	_best_fit_allocator = new D3D12BestFitAllocator(this);
}

D3D12ResourceAllocator::~D3D12ResourceAllocator()
{
	delete _best_fit_allocator;
}

ID3D12Resource* D3D12ResourceAllocator::get_resource(D3D12Allocation allocation)
{
	return _resource_heaps[allocation.heap].Get();
}

D3D12_GPU_VIRTUAL_ADDRESS D3D12ResourceAllocator::get_virtual_address(D3D12Allocation allocation)
{
	ID3D12Resource* resource = get_resource(allocation);
	return (D3D12_GPU_VIRTUAL_ADDRESS)(resource->GetGPUVirtualAddress() + allocation.offset * _desc.slot_size);
}

D3D12_GPU_VIRTUAL_ADDRESS D3D12ResourceAllocator::get_offset(D3D12Allocation allocation)
{
	return (D3D12_GPU_VIRTUAL_ADDRESS)(allocation.offset * _desc.slot_size);
}

D3D12Allocation D3D12ResourceAllocator::allocate(unsigned size_in_bytes)
{
	unsigned aligned_size = ALIGN(size_in_bytes, _desc.alignment);
	unsigned slot_count = aligned_size / _desc.slot_size;
	return _best_fit_allocator->allocate(slot_count);
}

void D3D12ResourceAllocator::deallocate(D3D12Allocation allocation)
{
	_best_fit_allocator->deallocate(allocation);
}

D3D12HeapHandle D3D12ResourceAllocator::create_heap()
{

	ComPtr<ID3D12Resource> resource;
	_device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(_desc.type),
		_desc.flags,
		&CD3DX12_RESOURCE_DESC::Buffer(_desc.size),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resource));
	D3D12HeapHandle heap_handle = (D3D12HeapHandle)_resource_heaps.size();
	_resource_heaps.push_back(resource);
	return heap_handle;
}

void D3D12ResourceAllocator::destroy_heap(D3D12HeapHandle handle)
{
	// Impl?
}

unsigned D3D12ResourceAllocator::slot_count()
{
	return _desc.size / _desc.slot_size;
}


}