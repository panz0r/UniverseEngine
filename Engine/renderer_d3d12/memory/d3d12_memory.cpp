#include "d3d12_memory.h"
#include <core/assert/assert.h>
#include <renderer_d3d12/renderer/d3d12_render_device.h>

namespace ue
{

D3D12Memory::D3D12Memory(D3D12RenderDevice& device)
{
	initialize(device);
}

D3D12Memory::~D3D12Memory()
{

}


void D3D12Memory::initialize(D3D12RenderDevice& device)
{

	{
		// 64mb upload buffer
		_upload_allocator = new D3D12RingBufferAllocator(device, 1024*1024*64);
	}

	{
		D3D12HeapAllocatorDesc desc = {};
		desc.size = 1024 * 1024 * 128;
		desc.slot_size = 1024 * 64;
		desc.type = D3D12_HEAP_TYPE_DEFAULT;
		desc.flags = D3D12_HEAP_FLAG_NONE;
		desc.allocator_type = ALLOCATOR_DEFAULT_HEAP;
		_default_heap_allocator = new D3D12HeapAllocator(device.device(), desc);
		_allocators[ALLOCATOR_DEFAULT_HEAP] = _default_heap_allocator;
	}

	{
		D3D12HeapAllocatorDesc desc = {};
		desc.size = 1024 * 1024 * 8;
		desc.slot_size = 1024 * 64;
		desc.type = D3D12_HEAP_TYPE_UPLOAD;
		desc.flags = D3D12_HEAP_FLAG_NONE;
		desc.allocator_type = ALLOCATOR_UPLOAD_HEAP;
		_upload_heap_allocator = new D3D12HeapAllocator(device.device(), desc);
		_allocators[ALLOCATOR_UPLOAD_HEAP] = _upload_heap_allocator;
	}

	{
		D3D12SubresourceAllocatorDesc desc = {};
		desc.max_resource_size = 1024 * 64 * 4;
		desc.alignment = 256;
		desc.resource_state = D3D12_RESOURCE_STATE_GENERIC_READ;
		desc.allocator_type = ALLOCATOR_DEFAULT_SUBRESOURCE;
		desc.can_grow = true;
		desc.mappable = false;
		_default_subresource_allocator = new D3D12SubresourceAllocator(device.device(), desc, _default_heap_allocator);
		_allocators[ALLOCATOR_DEFAULT_SUBRESOURCE] = _default_subresource_allocator;
	}

	{
		D3D12SubresourceAllocatorDesc desc = {};
		desc.max_resource_size = 1024 * 64 * 4;
		desc.alignment = 256;
		desc.resource_state = D3D12_RESOURCE_STATE_GENERIC_READ;
		desc.allocator_type = ALLOCATOR_UPLOAD_SUBRESOURCE;
		desc.can_grow = true;
		desc.mappable = true;
		_upload_subresource_allocator = new D3D12SubresourceAllocator(device.device(), desc, _upload_heap_allocator);
		_allocators[ALLOCATOR_UPLOAD_SUBRESOURCE] = _upload_subresource_allocator;
	}

	{
		D3D12DescriptorAllocatorDesc desc = {};
		desc.size = 128;
		desc.flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		_srv_cbv_uav_allocator = new D3D12DescriptorAllocator(device.device(), desc);
		_allocators[ALLOCATOR_SRV_CBV_UAV_DESCRIPTOR_HEAP] = _srv_cbv_uav_allocator;
	}

	{
		D3D12DescriptorAllocatorDesc desc = {};
		desc.size = 32;
		desc.flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		_rtv_allocator = new D3D12DescriptorAllocator(device.device(), desc);
		_allocators[ALLOCATOR_RTV_DESCRIPTOR_HEAP] = _rtv_allocator;
	}

	{
		D3D12DescriptorAllocatorDesc desc = {};
		desc.size = 8;
		desc.flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		_dsv_allocator = new D3D12DescriptorAllocator(device.device(), desc);
		_allocators[ALLOCATOR_DSV_DESCRIPTOR_HEAP] = _dsv_allocator;
	}

}

D3D12HeapAllocatorInterface* D3D12Memory::get_allocator(unsigned allocator_type)
{
	UENSURE(allocator_type < ALLOCATOR_COUNT);
	return _allocators[allocator_type];
}

D3D12HeapAccess D3D12Memory::get_heap_access(const D3D12Allocation& allocation)
{
	D3D12HeapAccess access;
	switch (allocation.allocator_type) {
		case ALLOCATOR_DEFAULT_HEAP:
		case ALLOCATOR_UPLOAD_HEAP:
		{
			D3D12HeapAllocator* allocator = reinterpret_cast<D3D12HeapAllocator*>(_allocators[allocation.allocator_type]);
			access.heap = allocator->get_heap(allocation);
			access.offset = allocator->get_offset(allocation);
			return access;
		}
	}
	UASSERT(false, "invalid allocation type");
	return D3D12HeapAccess();
}

D3D12SubResourceAccess D3D12Memory::get_subresource_access(const D3D12Allocation& allocation)
{
	switch (allocation.allocator_type) {
		case ALLOCATOR_DEFAULT_SUBRESOURCE:
		case ALLOCATOR_UPLOAD_SUBRESOURCE:
		{
			D3D12SubresourceAllocator* allocator = reinterpret_cast<D3D12SubresourceAllocator*>(_allocators[allocation.allocator_type]);
			return allocator->get_subresource_access(allocation);
		}
	}
	UASSERT(false, "invalid allocation type");
	return D3D12SubResourceAccess();
}

D3D12CPUDescriptorHandle D3D12Memory::get_cpu_descriptor_handle(const D3D12Allocation& allocation, unsigned offset)
{
	switch (allocation.allocator_type)
	{
		case ALLOCATOR_SRV_CBV_UAV_DESCRIPTOR_HEAP:
		case ALLOCATOR_RTV_DESCRIPTOR_HEAP:
		case ALLOCATOR_DSV_DESCRIPTOR_HEAP:
		case ALLOCATOR_SAMPLER_DESCRIPTOR_HEAP:
			return reinterpret_cast<D3D12DescriptorAllocator*>(_allocators[allocation.allocator_type])->get_cpu_handle(allocation, offset);
	}
	UASSERT(false, "invalid allocation type");
	return D3D12CPUDescriptorHandle();
}

D3D12GPUDescriptorHandle D3D12Memory::get_gpu_descriptor_handle(const D3D12Allocation& allocation, unsigned offset)
{
	switch (allocation.allocator_type)
	{
		case ALLOCATOR_SRV_CBV_UAV_DESCRIPTOR_HEAP:
		case ALLOCATOR_RTV_DESCRIPTOR_HEAP:
		case ALLOCATOR_DSV_DESCRIPTOR_HEAP:
		case ALLOCATOR_SAMPLER_DESCRIPTOR_HEAP:
			return reinterpret_cast<D3D12DescriptorAllocator*>(_allocators[allocation.allocator_type])->get_gpu_handle(allocation, offset);
	}
	UASSERT(false, "invalid allocation type");
	return D3D12GPUDescriptorHandle();
}

namespace d3d12_memory {

namespace internal {
	D3D12Memory* __memory = nullptr;
}


void initialize(D3D12RenderDevice& device)
{
	internal::__memory = new D3D12Memory(device);
}

void shutdown()
{
	delete internal::__memory;
}

D3D12HeapAllocatorInterface* allocator(unsigned allocator_type)
{
	return internal::__memory->get_allocator(allocator_type);
}

D3D12HeapAllocator* default_heap_allocator()
{
	return (D3D12HeapAllocator*)internal::__memory->get_allocator(ALLOCATOR_DEFAULT_HEAP);
}

D3D12HeapAllocator* upload_heap_allocator()
{
	return (D3D12HeapAllocator*)internal::__memory->get_allocator(ALLOCATOR_UPLOAD_HEAP);
}

D3D12SubresourceAllocator* default_subresource_allocator()
{
	return (D3D12SubresourceAllocator*)internal::__memory->get_allocator(ALLOCATOR_DEFAULT_SUBRESOURCE);
}

D3D12SubresourceAllocator* upload_subresource_allocator()
{
	return (D3D12SubresourceAllocator*)internal::__memory->get_allocator(ALLOCATOR_UPLOAD_SUBRESOURCE);
}

D3D12RingBufferAllocator* upload_ringbuffer_allocator()
{
	return internal::__memory->upload_ringbuffer_allocator();
}

D3D12HeapAccess heap_access(const D3D12Allocation& allocation)
{
	return internal::__memory->get_heap_access(allocation);
}

D3D12SubResourceAccess subresource_access(const D3D12Allocation& allocation)
{
	return internal::__memory->get_subresource_access(allocation);
}

D3D12CPUDescriptorHandle cpu_descriptor_handle(const D3D12Allocation& allocation, unsigned offset)
{
	return internal::__memory->get_cpu_descriptor_handle(allocation, offset);
}

D3D12GPUDescriptorHandle gpu_descriptor_handle(const D3D12Allocation& allocation, unsigned offset)
{
	return internal::__memory->get_gpu_descriptor_handle(allocation, offset);
}


}

}