#include "d3d12_descriptor_allocator.h"
#include "d3d12_best_fit_allocator.h"
#include <core/assert/assert.h>

namespace ue
{

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

D3D12_CPU_DESCRIPTOR_HANDLE D3D12DescriptorAllocator::get_cpu_handle(const D3D12DescriptorAllocation& allocation, unsigned offset)
{
	UENSURE(offset < allocation.count);
	ID3D12DescriptorHeap* descriptor_heap = _descriptor_heaps[allocation.heap].Get();
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptor_heap->GetCPUDescriptorHandleForHeapStart(), allocation.offset + offset, _descriptor_increment_size);
}

D3D12_GPU_DESCRIPTOR_HANDLE D3D12DescriptorAllocator::get_gpu_handle(const D3D12DescriptorAllocation& allocation, unsigned offset)
{
	UENSURE(offset < allocation.count);
	ID3D12DescriptorHeap* descriptor_heap = _descriptor_heaps[allocation.heap].Get();
	return CD3DX12_GPU_DESCRIPTOR_HANDLE(descriptor_heap->GetGPUDescriptorHandleForHeapStart(), allocation.offset + offset, _descriptor_increment_size);
}

D3D12DescriptorAllocation D3D12DescriptorAllocator::allocate(unsigned descriptor_count, unsigned align)
{
	_allocated_count += descriptor_count;
	return _best_fit_allocator->allocate(descriptor_count);
}

void D3D12DescriptorAllocator::deallocate(const D3D12DescriptorAllocation& allocation)
{
	_allocated_count -= allocation.count;
	_best_fit_allocator->deallocate(allocation);
}

unsigned D3D12DescriptorAllocator::get_total_size()
{
	return (unsigned)_descriptor_heaps.size() * _desc.size;
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