#pragma once
#include <renderer_d3d12/stdinclude.h>
#include "d3d12_allocator_interface.h"
#include <vector>

namespace ue
{

////////////////////////////////////////////////////////////////////

struct D3D12HeapAllocatorDesc
{
	unsigned size;
	unsigned slot_size;
	D3D12_HEAP_TYPE type;
	D3D12_HEAP_FLAGS flags;
	unsigned allocator_type;
};

////////////////////////////////////////////////////////////////////

class D3D12HeapAllocator sealed : public D3D12HeapAllocatorInterface
{
public:
	D3D12HeapAllocator(ID3D12Device* device, const D3D12HeapAllocatorDesc &desc);
	~D3D12HeapAllocator();

	ID3D12Heap* get_heap(const D3D12Allocation& allocation);
	D3D12_GPU_VIRTUAL_ADDRESS get_offset(const D3D12Allocation& allocation);

	D3D12Allocation allocate(unsigned size_in_bytes, unsigned align = 0U);
	void deallocate(const D3D12Allocation& allocation);

private:
	virtual D3D12HeapHandle create_heap() override;
	virtual void destroy_heap(D3D12HeapHandle handle) override;
	virtual unsigned slot_count() override;

private:
	ID3D12Device* _device;
	D3D12HeapAllocatorDesc _desc;
	D3D12AllocatorInterface* _best_fit_allocator;
	std::vector<ComPtr<ID3D12Heap>> _heaps;
};


}