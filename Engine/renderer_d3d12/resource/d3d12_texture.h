#pragma once

#include "d3d12_render_resource.h"
#include "placed_gpu_resource.h"
#include "../com/com_ptr.h"

namespace ue
{

struct D3D12Texture : public D3D12RenderResource
{
	PlacedGPUResource placed_gpu_resource;

	OfflineDescriptorHeapHandle offline_descriptor_handles[2];
	D3D12_CPU_DESCRIPTOR_HANDLE srv_descriptor_handle;
	D3D12_CPU_DESCRIPTOR_HANDLE uav_descriptor_handle;
	D3D12_GPU_VIRTUAL_ADDRESS gpu_virtual_address;
	void * data;
	unsigned aligned_size;
	unsigned size;
};

struct D3D12RenderTarget : public D3D12RenderResource
{
	OfflineDescriptorHeapHandle offline_descriptor_handle[2];
	D3D12_CPU_DESCRIPTOR_HANDLE srv_descriptor_handle;
	D3D12_CPU_DESCRIPTOR_HANDLE uav_descriptor_handle;
	union
	{
		D3D12_CPU_DESCRIPTOR_HANDLE rtv_descriptor_handle;
		D3D12_CPU_DESCRIPTOR_HANDLE dsv_descriptor_handle;
	};

	ComPtr<ID3D12Resource> d3d_resource;
	unsigned aligned_size;
	unsigned size;
};


}