#pragma once

#include <renderer_d3d12/stdinclude.h>
#include "d3d12_allocator_interface.h"
#include <vector>

namespace ue
{

////////////////////////////////////////////////////////////////////

struct D3D12ResourceAllocatorDesc
{
	unsigned size;
	unsigned slot_size;
	D3D12_HEAP_TYPE type;
	D3D12_HEAP_FLAGS flags;
};

////////////////////////////////////////////////////////////////////

class D3D12ResourceAllocator sealed : public D3D12HeapAllocatorInterface
{
public:
	D3D12ResourceAllocator(ID3D12Device* device, const D3D12ResourceAllocatorDesc &desc);
	~D3D12ResourceAllocator();

	ID3D12Resource* get_resource(const D3D12Allocation &allocation);
	D3D12_GPU_VIRTUAL_ADDRESS get_offset(const D3D12Allocation& allocation);
	D3D12_GPU_VIRTUAL_ADDRESS get_virtual_address(const D3D12Allocation& allocation);
	void* get_mapped_data(const D3D12Allocation& allocation);

	D3D12Allocation allocate(unsigned size_in_bytes, unsigned align = 0U);
	void deallocate(const D3D12Allocation& allocation);

private:
	virtual D3D12HeapHandle create_heap() override;
	virtual void destroy_heap(D3D12HeapHandle handle) override;
	virtual unsigned slot_count() override;

private:
	ID3D12Device* _device;
	D3D12ResourceAllocatorDesc _desc;
	D3D12AllocatorInterface* _best_fit_allocator;

	struct ResourceHeap
	{
		ComPtr<ID3D12Resource> resource_heap;
		void* mapped_data;
	};
	std::vector<ResourceHeap> _resource_heaps;
};


}