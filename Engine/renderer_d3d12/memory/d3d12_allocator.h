#pragma once


#include <d3d12.h>
#include <vector>
#include "../com/com_ptr.h"
#include "d3d12_allocator_interface.h"
#include "d3d12_best_fit_allocator.h"

namespace ue
{

struct D3D12AllocatorDesc
{
	unsigned size;
	unsigned alignment;
	unsigned slot_size;
	D3D12_HEAP_TYPE type;
	D3D12_HEAP_FLAGS flags;
};

////////////////////////////////////////////////////////////////////

typedef D3D12Allocation D3D12HeapAllocation;

class D3D12HeapAllocator sealed : public D3D12HeapAllocatorInterface
{
public:
	D3D12HeapAllocator(ID3D12Device* device, const D3D12AllocatorDesc &desc);
	~D3D12HeapAllocator();

	ID3D12Heap* get_heap(const D3D12Allocation& allocation);
	D3D12_GPU_VIRTUAL_ADDRESS get_offset(const D3D12Allocation& allocation);

	D3D12Allocation allocate(unsigned size_in_bytes);
	void deallocate(const D3D12Allocation& allocation);

private:
	virtual D3D12HeapHandle create_heap() override;
	virtual void destroy_heap(D3D12HeapHandle handle) override;
	virtual unsigned slot_count() override;

private:
	ID3D12Device* _device;
	D3D12AllocatorDesc _desc;
	D3D12BestFitAllocator* _best_fit_allocator;
	std::vector<ComPtr<ID3D12Heap>> _heaps;
};

////////////////////////////////////////////////////////////////////

typedef D3D12Allocation D3D12ResourceAllocation;

class D3D12ResourceAllocator sealed : public D3D12HeapAllocatorInterface
{
public:
	D3D12ResourceAllocator(ID3D12Device* device, const D3D12AllocatorDesc &desc);
	~D3D12ResourceAllocator();

	ID3D12Resource* get_resource(const D3D12ResourceAllocation &allocation);
	D3D12_GPU_VIRTUAL_ADDRESS get_offset(const D3D12ResourceAllocation& allocation);
	D3D12_GPU_VIRTUAL_ADDRESS get_virtual_address(const D3D12ResourceAllocation& allocation);
	void* get_mapped_data(const D3D12ResourceAllocation& allocation);

	D3D12ResourceAllocation allocate(unsigned size_in_bytes);
	void deallocate(const D3D12ResourceAllocation& allocation);

private:
	virtual D3D12HeapHandle create_heap() override;
	virtual void destroy_heap(D3D12HeapHandle handle) override;
	virtual unsigned slot_count() override;

private:
	ID3D12Device* _device;
	D3D12AllocatorDesc _desc;
	D3D12BestFitAllocator* _best_fit_allocator;

	struct ResourceHeap
	{
		ComPtr<ID3D12Resource> resource_heap;
		void* mapped_data;
	};
	std::vector<ResourceHeap> _resource_heaps;
};

////////////////////////////////////////////////////////////////////

typedef D3D12Allocation D3D12DescriptorAllocation;

struct D3D12DescriptorAllocatorDesc
{
	unsigned size;
	D3D12_DESCRIPTOR_HEAP_TYPE type;
	D3D12_DESCRIPTOR_HEAP_FLAGS flags;
};

class D3D12DescriptorAllocator sealed : public D3D12HeapAllocatorInterface
{
public:
	D3D12DescriptorAllocator(ID3D12Device* device, const D3D12DescriptorAllocatorDesc &desc);
	~D3D12DescriptorAllocator();

	D3D12_CPU_DESCRIPTOR_HANDLE get_cpu_handle(const D3D12Allocation& allocation, unsigned offset = 0);
	D3D12_GPU_DESCRIPTOR_HANDLE get_gpu_handle(const D3D12Allocation& allocation, unsigned offset = 0);

	D3D12Allocation allocate(unsigned descriptor_count);
	void deallocate(const D3D12Allocation& allocation);

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
	D3D12BestFitAllocator* _best_fit_allocator;
	std::vector<ComPtr<ID3D12DescriptorHeap>> _descriptor_heaps;

};

}