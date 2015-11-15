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
	PlacedGPUResource placed_resource;
	DescriptorHeapHandle *handles;
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

}