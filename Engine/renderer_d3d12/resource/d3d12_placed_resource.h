#pragma once

#include <d3d12.h>
#include <renderer_d3d12/com/com_ptr.h>
#include <renderer_d3d12/memory/d3d12_allocator.h>

namespace ue
{

struct D3D12PlacedResource
{
	D3D12HeapAllocation allocation;
	ComPtr<ID3D12Resource> resource;
};

}