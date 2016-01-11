#pragma once

#include "d3d12_best_fit_allocator.h"

#include <core/assert/assert.h>
#include <renderer_d3d12/resource/d3d12_resource.h>
#include <atomic>

namespace ue
{

class D3D12HeapAllocator;

struct D3D12SubresourceAllocatorDesc
{
	unsigned max_resource_size;
	unsigned alignment;
	unsigned resource_state;
	unsigned allocator_type;
	bool mappable;
	bool can_grow;
};

struct MappedData
{
	std::atomic<unsigned> map_count;
	void* base_mapping;
	void* map(size_t offset = 0) { map_count++; return reinterpret_cast<void*>(reinterpret_cast<size_t>(base_mapping) + offset); }
	void unmap() { map_count--; UENSURE(map_count > 0); }
};

struct D3D12SubResourceAccess
{
	D3D12Resource resource;
	D3D12_GPU_VIRTUAL_ADDRESS virtual_address;
	size_t resource_offset;
	MappedData* mapped_data;
};

class D3D12SubresourceAllocator : public D3D12HeapAllocatorInterface
{
public:
	D3D12SubresourceAllocator(ID3D12Device* device, const D3D12SubresourceAllocatorDesc& desc, D3D12HeapAllocator* backing_heap_allocator);
	D3D12SubresourceAllocator(ID3D12Device* device, const D3D12SubresourceAllocatorDesc& desc, ComPtr<ID3D12Resource>& backing_resource);
	~D3D12SubresourceAllocator();

	D3D12Allocation allocate(unsigned size_in_bytes, unsigned align);
	void deallocate(const D3D12Allocation& allocation);
	
	D3D12SubResourceAccess get_subresource_access(const D3D12Allocation& allocation);

	//D3D12_GPU_VIRTUAL_ADDRESS get_gpu_virtual_address(const D3D12Allocation& allocation);
	//ID3D12Resource* get_resource(const D3D12Allocation& allocation);

private:

	virtual D3D12HeapHandle create_heap();
	virtual void destroy_heap(D3D12HeapHandle handle);
	virtual unsigned slot_count();

private:
	ID3D12Device* _device;
	D3D12HeapAllocator* _backing_heap_allocator;
	D3D12BestFitAllocator* _best_fit_allocator;
	D3D12SubresourceAllocatorDesc _desc;

	struct ResourceHeap
	{
		D3D12Allocation heap_allocation;
		ComPtr<ID3D12Resource> resource;
		MappedData* mapped_data;
	};

	std::vector<ResourceHeap> _heap_allocations;
};

}