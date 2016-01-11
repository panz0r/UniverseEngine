#pragma once

#include "d3d12_heap_allocator.h"
#include "d3d12_comitted_allocator.h"
#include "d3d12_descriptor_allocator.h"
#include "d3d12_subresource_allocator.h"
#include "d3d12_ring_allocator.h"

namespace ue {

class D3D12RenderDevice;

struct D3D12HeapAccess
{
	ID3D12Heap* heap;
	D3D12HeapOffset offset;
};

typedef D3D12_CPU_DESCRIPTOR_HANDLE D3D12CPUDescriptorHandle;
typedef D3D12_GPU_DESCRIPTOR_HANDLE D3D12GPUDescriptorHandle;


enum D3D12_ALLOCATOR_TYPES {
	ALLOCATOR_DEFAULT_HEAP,
	ALLOCATOR_UPLOAD_HEAP,
	ALLOCATOR_DEFAULT_SUBRESOURCE,
	ALLOCATOR_UPLOAD_SUBRESOURCE,
	ALLOCATOR_SRV_CBV_UAV_DESCRIPTOR_HEAP,
	ALLOCATOR_RTV_DESCRIPTOR_HEAP,
	ALLOCATOR_DSV_DESCRIPTOR_HEAP,
	ALLOCATOR_SAMPLER_DESCRIPTOR_HEAP,

	ALLOCATOR_COUNT
};

class D3D12Memory
{
public:

	D3D12Memory(D3D12RenderDevice& device);
	~D3D12Memory();

	void initialize(D3D12RenderDevice& device);
	D3D12HeapAllocatorInterface* get_allocator(unsigned allocator_type);
	D3D12HeapAccess get_heap_access(const D3D12Allocation& allocation);
	D3D12SubResourceAccess get_subresource_access(const D3D12Allocation& allocation);
	D3D12CPUDescriptorHandle get_cpu_descriptor_handle(const D3D12Allocation& allocation, unsigned offset);
	D3D12GPUDescriptorHandle get_gpu_descriptor_handle(const D3D12Allocation& allocation, unsigned offset);

	D3D12RingBufferAllocator* upload_ringbuffer_allocator() { return _upload_allocator; }

private:
	
	D3D12HeapAllocator* _default_heap_allocator;
	D3D12HeapAllocator* _upload_heap_allocator;
	D3D12SubresourceAllocator* _default_subresource_allocator;
	D3D12SubresourceAllocator* _upload_subresource_allocator;

	D3D12DescriptorAllocator* _srv_cbv_uav_allocator;
	D3D12DescriptorAllocator* _rtv_allocator;
	D3D12DescriptorAllocator* _dsv_allocator;
	D3D12DescriptorAllocator* _sampler_allocator;

	D3D12HeapAllocatorInterface* _allocators[ALLOCATOR_COUNT];

	D3D12RingBufferAllocator* _upload_allocator;
	//D3D12DescriptorAllocator* _online_descriptor_allocator;
};

namespace d3d12_memory
{

void initialize(D3D12RenderDevice& device);
void shutdown();

D3D12HeapAllocatorInterface* allocator(unsigned allocator_type);

D3D12HeapAllocator* default_heap_allocator();
D3D12HeapAllocator* upload_heap_allocator();
D3D12SubresourceAllocator* upload_subresource_allocator();
D3D12SubresourceAllocator* default_subresource_allocator();
D3D12RingBufferAllocator* upload_ringbuffer_allocator();


D3D12HeapAccess heap_access(const D3D12Allocation& allocation);
D3D12SubResourceAccess subresource_access(const D3D12Allocation& allocation);
D3D12CPUDescriptorHandle cpu_descriptor_handle(const D3D12Allocation& allocation, unsigned offset = 0U);
D3D12GPUDescriptorHandle gpu_descriptor_handle(const D3D12Allocation& allocation, unsigned offset = 0U);


}


}