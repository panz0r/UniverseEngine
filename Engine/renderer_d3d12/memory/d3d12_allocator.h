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

class D3D12HeapAllocator sealed : public D3D12HeapAllocatorInterface
{
public:
	D3D12HeapAllocator(ID3D12Device* device, const D3D12AllocatorDesc &desc);
	~D3D12HeapAllocator();

	ID3D12Heap* get_heap(D3D12Allocation allocation);
	D3D12_GPU_VIRTUAL_ADDRESS get_offset(D3D12Allocation allocation);

	D3D12Allocation allocate(unsigned size_in_bytes);
	void deallocate(D3D12Allocation allocation);

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


class D3D12ResourceAllocator sealed : public D3D12HeapAllocatorInterface
{
public:
	D3D12ResourceAllocator(ID3D12Device* device, const D3D12AllocatorDesc &desc);
	~D3D12ResourceAllocator();

	ID3D12Resource* get_resource(D3D12Allocation allocation);
	D3D12_GPU_VIRTUAL_ADDRESS get_offset(D3D12Allocation allocation);
	D3D12_GPU_VIRTUAL_ADDRESS get_virtual_address(D3D12Allocation allocation);

	D3D12Allocation allocate(unsigned size_in_bytes);
	void deallocate(D3D12Allocation allocation);

private:
	virtual D3D12HeapHandle create_heap() override;
	virtual void destroy_heap(D3D12HeapHandle handle) override;
	virtual unsigned slot_count() override;

private:
	ID3D12Device* _device;
	D3D12AllocatorDesc _desc;
	D3D12BestFitAllocator* _best_fit_allocator;
	std::vector<ComPtr<ID3D12Resource>> _resource_heaps;
};



}