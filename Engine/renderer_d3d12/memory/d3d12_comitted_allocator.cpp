#include "d3d12_comitted_allocator.h"
#include "d3d12_best_fit_allocator.h"

namespace ue
{

D3D12ResourceAllocator::D3D12ResourceAllocator(ID3D12Device* device, const D3D12ResourceAllocatorDesc& desc)
	: _device(device)
	, _desc(desc)
{
	_best_fit_allocator = new D3D12BestFitAllocator(this);
}

D3D12ResourceAllocator::~D3D12ResourceAllocator()
{
	delete _best_fit_allocator;
}

ID3D12Resource* D3D12ResourceAllocator::get_resource(const D3D12Allocation& allocation)
{
	return _resource_heaps[allocation.heap].resource_heap.Get();
}

D3D12_GPU_VIRTUAL_ADDRESS D3D12ResourceAllocator::get_virtual_address(const D3D12Allocation& allocation)
{
	ID3D12Resource* resource = get_resource(allocation);
	return (D3D12_GPU_VIRTUAL_ADDRESS)(resource->GetGPUVirtualAddress() + allocation.offset * _desc.slot_size);
}

D3D12_GPU_VIRTUAL_ADDRESS D3D12ResourceAllocator::get_offset(const D3D12Allocation& allocation)
{
	return (D3D12_GPU_VIRTUAL_ADDRESS)(allocation.offset * _desc.slot_size);
}

void* D3D12ResourceAllocator::get_mapped_data(const D3D12Allocation& allocation)
{
	ResourceHeap* heap = &_resource_heaps[allocation.heap];
	if (heap->mapped_data == nullptr)
		return nullptr;
	return reinterpret_cast<void*>(reinterpret_cast<size_t>(heap->mapped_data) + allocation.offset * _desc.slot_size);
}

D3D12Allocation D3D12ResourceAllocator::allocate(unsigned size_in_bytes, unsigned align)
{
	unsigned slot_count = size_in_bytes / _desc.slot_size;
	return _best_fit_allocator->allocate(slot_count);
}

void D3D12ResourceAllocator::deallocate(const D3D12Allocation& allocation)
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

}