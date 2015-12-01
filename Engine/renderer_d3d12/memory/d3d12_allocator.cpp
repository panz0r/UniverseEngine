#include "d3d12_allocator.h"
#include "../renderer/d3dx12.h"
#include <core/assert/assert.h>

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

ID3D12Heap* D3D12HeapAllocator::get_heap(const D3D12Allocation& allocation)
{
	return _heaps[allocation.heap].Get();
}

D3D12_GPU_VIRTUAL_ADDRESS D3D12HeapAllocator::get_offset(const D3D12Allocation& allocation)
{
	return (D3D12_GPU_VIRTUAL_ADDRESS)(allocation.offset * _desc.slot_size);
}

D3D12Allocation D3D12HeapAllocator::allocate(unsigned size_in_bytes)
{
	unsigned aligned_size = ALIGN(size_in_bytes, _desc.alignment);
	unsigned slot_count = aligned_size / _desc.slot_size;
	return _best_fit_allocator->allocate(slot_count);
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

ID3D12Resource* D3D12ResourceAllocator::get_resource(const D3D12ResourceAllocation& allocation)
{
	return _resource_heaps[allocation.heap].resource_heap.Get();
}

D3D12_GPU_VIRTUAL_ADDRESS D3D12ResourceAllocator::get_virtual_address(const D3D12ResourceAllocation& allocation)
{
	ID3D12Resource* resource = get_resource(allocation);
	return (D3D12_GPU_VIRTUAL_ADDRESS)(resource->GetGPUVirtualAddress() + allocation.offset * _desc.slot_size);
}

D3D12_GPU_VIRTUAL_ADDRESS D3D12ResourceAllocator::get_offset(const D3D12ResourceAllocation& allocation)
{
	return (D3D12_GPU_VIRTUAL_ADDRESS)(allocation.offset * _desc.slot_size);
}

void* D3D12ResourceAllocator::get_mapped_data(const D3D12ResourceAllocation& allocation)
{
	ResourceHeap* heap = &_resource_heaps[allocation.heap];
	if (heap->mapped_data == nullptr)
		return nullptr;
	return reinterpret_cast<void*>(reinterpret_cast<size_t>(heap->mapped_data) + allocation.offset * _desc.slot_size);
}

D3D12ResourceAllocation D3D12ResourceAllocator::allocate(unsigned size_in_bytes)
{
	unsigned aligned_size = ALIGN(size_in_bytes, _desc.alignment);
	unsigned slot_count = aligned_size / _desc.slot_size;
	return _best_fit_allocator->allocate(slot_count);
}

void D3D12ResourceAllocator::deallocate(const D3D12ResourceAllocation& allocation)
{
	_best_fit_allocator->deallocate(allocation);
}

D3D12HeapHandle D3D12ResourceAllocator::create_heap()
{

	ResourceHeap heap = {};
	_device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(_desc.type),
		_desc.flags,
		&CD3DX12_RESOURCE_DESC::Buffer(_desc.size),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&heap.resource_heap));
	if (_desc.type == D3D12_HEAP_TYPE_UPLOAD) {
		heap.resource_heap->Map(0, nullptr, &heap.mapped_data);
	}
	D3D12HeapHandle heap_handle = (D3D12HeapHandle)_resource_heaps.size();
	_resource_heaps.push_back(heap);
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

////////////////////////////////////////////////////////////////////


D3D12DescriptorAllocator::D3D12DescriptorAllocator(ID3D12Device* device, const D3D12DescriptorAllocatorDesc& desc)
	: _device(device)
	, _desc(desc)
	, _allocated_count(0)
{
	_descriptor_increment_size = device->GetDescriptorHandleIncrementSize(desc.type);
	_best_fit_allocator = new D3D12BestFitAllocator(this);
}

D3D12DescriptorAllocator::~D3D12DescriptorAllocator()
{
}

D3D12_CPU_DESCRIPTOR_HANDLE D3D12DescriptorAllocator::get_cpu_handle(const D3D12Allocation& allocation, unsigned offset)
{
	UENSURE(offset < allocation.count);
	ID3D12DescriptorHeap* descriptor_heap = _descriptor_heaps[allocation.heap].Get();
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptor_heap->GetCPUDescriptorHandleForHeapStart(), allocation.offset + offset, _descriptor_increment_size);
}

D3D12_GPU_DESCRIPTOR_HANDLE D3D12DescriptorAllocator::get_gpu_handle(const D3D12Allocation& allocation, unsigned offset)
{
	UENSURE(offset < allocation.count);
	ID3D12DescriptorHeap* descriptor_heap = _descriptor_heaps[allocation.heap].Get();
	return CD3DX12_GPU_DESCRIPTOR_HANDLE(descriptor_heap->GetGPUDescriptorHandleForHeapStart(), allocation.offset + offset, _descriptor_increment_size);
}

D3D12Allocation D3D12DescriptorAllocator::allocate(unsigned descriptor_count)
{
	_allocated_count += descriptor_count;
	return _best_fit_allocator->allocate(descriptor_count);
}

void D3D12DescriptorAllocator::deallocate(const D3D12Allocation& allocation)
{
	_allocated_count -= allocation.count;
	_best_fit_allocator->deallocate(allocation);
}

unsigned D3D12DescriptorAllocator::get_total_size()
{
	return _descriptor_heaps.size() * _desc.size;
}

unsigned D3D12DescriptorAllocator::get_free_size()
{
	return get_total_size() - _allocated_count;
}

unsigned D3D12DescriptorAllocator::get_allocated_size()
{
	return _allocated_count;
}

D3D12HeapHandle D3D12DescriptorAllocator::create_heap()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NodeMask = 0;
	desc.NumDescriptors = _desc.size;
	desc.Flags = _desc.flags;
	desc.Type = _desc.type;
	ComPtr<ID3D12DescriptorHeap> descriptor_heap;
	_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptor_heap));
	D3D12HeapHandle heap_handle = (D3D12HeapHandle)_descriptor_heaps.size();
	_descriptor_heaps.push_back(descriptor_heap);
	return heap_handle;
}

void D3D12DescriptorAllocator::destroy_heap(D3D12HeapHandle handle)
{
	// impl?
}

unsigned D3D12DescriptorAllocator::slot_count()
{
	return _desc.size;
}

}