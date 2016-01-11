#pragma once

#include <renderer_d3d12/stdinclude.h>
#include "d3d12_allocator_interface.h"
#include <vector>


namespace ue
{

////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////

struct D3D12DescriptorAllocatorDesc
{
	unsigned size;
	D3D12_DESCRIPTOR_HEAP_TYPE type;
	D3D12_DESCRIPTOR_HEAP_FLAGS flags;
};

////////////////////////////////////////////////////////////////////

class D3D12DescriptorAllocator sealed : public D3D12HeapAllocatorInterface
{
public:
	D3D12DescriptorAllocator(ID3D12Device* device, const D3D12DescriptorAllocatorDesc &desc);
	~D3D12DescriptorAllocator();

	D3D12_CPU_DESCRIPTOR_HANDLE get_cpu_handle(const D3D12DescriptorAllocation& allocation, unsigned offset = 0);
	D3D12_GPU_DESCRIPTOR_HANDLE get_gpu_handle(const D3D12DescriptorAllocation& allocation, unsigned offset = 0);

	D3D12DescriptorAllocation allocate(unsigned descriptor_count, unsigned align = 0U);
	void deallocate(const D3D12DescriptorAllocation& allocation);

	unsigned get_total_size();
	unsigned get_free_size();
	unsigned get_allocated_size();

private:
	virtual D3D12HeapHandle create_heap() override;
	virtual void destroy_heap(D3D12HeapHandle handle) override;
	virtual unsigned slot_count() override;

private:
	ID3D12Device* _device;
	D3D12DescriptorAllocatorDesc _desc;
	unsigned _allocated_count;
	unsigned _descriptor_increment_size;
	D3D12AllocatorInterface* _best_fit_allocator;
	std::vector<ComPtr<ID3D12DescriptorHeap>> _descriptor_heaps;

};

}