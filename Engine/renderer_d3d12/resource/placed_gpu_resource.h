#pragma once

#include "common.h"
#include <core/memory/buddy_allocator_ext.h>

namespace ue
{

class PlacedGPUResource
{
public:
	PlacedGPUResource() : _allocator(nullptr), _heap_virtual_ptr(nullptr) {}
	PlacedGPUResource(BuddyAllocatorExt *allocator, unsigned size)
		: _allocator(allocator)
	{
		_heap_virtual_ptr = _allocator->allocate(size);
	}
	PlacedGPUResource(const PlacedGPUResource &other)
		: _allocator(other._allocator)
		, _heap_virtual_ptr(other._heap_virtual_ptr)
		, _resource(other._resource)
	{
	}

	~PlacedGPUResource()
	{
	}

	ID3D12Resource** init_resource() { return &_resource; }
	ID3D12Resource* resource() { return _resource.Get(); }
	uint64_t offset() { return reinterpret_cast<uint64_t>(_heap_virtual_ptr); }
	void deallocate() { _allocator->deallocate(_heap_virtual_ptr); }

private:
	ComPtr<ID3D12Resource> _resource;
	BuddyAllocatorExt *_allocator;
	void *_heap_virtual_ptr;
};


}