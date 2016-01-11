#include "d3d12_subresource_allocator.h"
#include "d3d12_heap_allocator.h"

#include <renderer_d3d12/d3dx12.h>
#include <core/assert/assert.h>

#define MAX(a,b) ((a>b) ? (a) : (b))

namespace ue
{

D3D12SubresourceAllocator::D3D12SubresourceAllocator(ID3D12Device* device, const D3D12SubresourceAllocatorDesc& desc, D3D12HeapAllocator* backing_heap_allocator)
	: _device(device)
	, _backing_heap_allocator(backing_heap_allocator)
	, _desc(desc)
{
	_best_fit_allocator = new D3D12BestFitAllocator(this);
}

D3D12SubresourceAllocator::D3D12SubresourceAllocator(ID3D12Device* device, const D3D12SubresourceAllocatorDesc& desc, ComPtr<ID3D12Resource>& backing_resource)
	: _device(device)
	, _backing_heap_allocator(nullptr)
	, _desc(desc)
{
	ResourceHeap allocation;
	allocation.heap_allocation = D3D12Allocation();
	allocation.resource = backing_resource;
	_heap_allocations.push_back(allocation);

	_best_fit_allocator = new D3D12BestFitAllocator(this);
}


D3D12SubresourceAllocator::~D3D12SubresourceAllocator()
{
	delete _best_fit_allocator;
}

D3D12Allocation D3D12SubresourceAllocator::allocate(unsigned size_in_bytes, unsigned align)
{
	UASSERT(size_in_bytes <= _desc.max_resource_size, "allocation too large");

	unsigned aligned_size_in_bytes = ALIGN(size_in_bytes, _desc.alignment);
	unsigned slot_count_to_allocate = MAX(aligned_size_in_bytes / _desc.alignment, 1U);
	D3D12Allocation allocation = _best_fit_allocator->allocate(slot_count_to_allocate, align);
	allocation.allocator_type = _desc.allocator_type;
	return allocation;
}


void D3D12SubresourceAllocator::deallocate(const D3D12Allocation& allocation)
{
	_best_fit_allocator->deallocate(allocation);
}

D3D12SubResourceAccess D3D12SubresourceAllocator::get_subresource_access(const D3D12Allocation& allocation)
{
	ResourceHeap& heap = _heap_allocations[allocation.heap];
	D3D12SubResourceAccess access;
	access.resource.resource = heap.resource;
	access.resource.resource_state = (D3D12_RESOURCE_STATES)_desc.resource_state;
	access.virtual_address = heap.resource->GetGPUVirtualAddress() + allocation.offset;
	access.resource_offset = allocation.offset;
	access.mapped_data = heap.mapped_data;
	return access;
}

D3D12HeapHandle D3D12SubresourceAllocator::create_heap()
{
	if(!_desc.can_grow)
		return (D3D12HeapHandle)0;

	D3D12Allocation heap_allocation = _backing_heap_allocator->allocate(_desc.max_resource_size);
	ID3D12Heap* heap = _backing_heap_allocator->get_heap(heap_allocation);
	D3D12HeapOffset heap_offset = _backing_heap_allocator->get_offset(heap_allocation);
	ComPtr<ID3D12Resource> resource;
	HRESULT hr = _device->CreatePlacedResource(heap,
		heap_offset,
		&CD3DX12_RESOURCE_DESC::Buffer(_desc.max_resource_size),
		(D3D12_RESOURCE_STATES)_desc.resource_state,
		nullptr,
		IID_PPV_ARGS(&resource));
	UENSURE(SUCCEEDED(hr));

	
	MappedData* mapped_data = nullptr;
	if (_desc.mappable) {
		mapped_data = new MappedData();
		mapped_data->map_count = 1;
		hr = resource->Map(0, nullptr, &mapped_data->base_mapping);
		UENSURE(SUCCEEDED(hr));
	}

	unsigned heap_index = _heap_allocations.size();
	
	ResourceHeap resource_allocation = {
		heap_allocation,
		resource,
		mapped_data
	};

	_heap_allocations.push_back(resource_allocation);
	return (D3D12HeapHandle)heap_index;
}

void D3D12SubresourceAllocator::destroy_heap(D3D12HeapHandle handle)
{
	
}

unsigned D3D12SubresourceAllocator::slot_count()
{
	return _desc.max_resource_size / _desc.alignment;
}



}