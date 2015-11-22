#pragma once

#include "common.h"
#include "placed_gpu_resource.h"

namespace ue
{

struct BufferDesc
{
	unsigned size;
	union {
		unsigned stride;
		unsigned format;
	};
	bool dynamic;
};



struct BufferResource : RenderResource
{
	static const ResourceType Type = RenderResource::Buffer;

	PlacedGPUResource placed_resource;
	OfflineDescriptorHeapHandle *handles;
	D3D12_CPU_DESCRIPTOR_HANDLE *srv;
	D3D12_CPU_DESCRIPTOR_HANDLE *uav;
	union
	{
		// These three are very unlikely that any one buffer is at the same time
		D3D12_VERTEX_BUFFER_VIEW *vbv;
		D3D12_INDEX_BUFFER_VIEW *ibv;
		D3D12_CPU_DESCRIPTOR_HANDLE *cbv;
	};
	void *mapped_data;
	unsigned size;
	unsigned stride;

	// srv/uav/vbv/ibv
	// mapped data
	// stride
	// size
};

struct ConstantBufferResource : public RenderResource
{
	static const ResourceType Type = RenderResource::ConstantBuffer;


	OfflineDescriptorHeapHandle *handle;
	D3D12_CPU_DESCRIPTOR_HANDLE *cbv;
	D3D12_GPU_VIRTUAL_ADDRESS *gpu_va;

	void *mapped_data;
	unsigned frame_map_stride;

	inline D3D12_CPU_DESCRIPTOR_HANDLE cbv_cpu_handle(unsigned frame_index) {
		return cbv[frame_index];
	}

	inline void* map(unsigned frame_index) {
		return reinterpret_cast<void*>(reinterpret_cast<size_t>(mapped_data) + frame_map_stride * frame_index);
	}

	virtual D3D12_GPU_VIRTUAL_ADDRESS gpu_virtual_address() { return gpu_va[0]; }
	virtual D3D12_CPU_DESCRIPTOR_HANDLE cpu_descriptor_handle() { return cbv[0]; }


	unsigned size;
};


}