#pragma once

#include "d3d12_render_resource.h"
#include "placed_gpu_resource.h"
#include "../renderer/offline_descriptor_heap.h"

namespace ue
{

// For all dynamic buffers allocated on an upload heap
// Normally double buffered and can be updated directly through the data mapping
struct D3D12DynamicBuffer : public D3D12RenderResource
{
	D3D12_GPU_VIRTUAL_ADDRESS gpu_virtual_address;
	union {
		OfflineDescriptorHeapHandle cbv;
		struct _SRV_UAV {
			OfflineDescriptorHeapHandle srv;
			OfflineDescriptorHeapHandle uav;
		} srv_uav;
	};
	void * data;
	unsigned aligned_size;
	unsigned size;
};

// For all static buffers allocted on a default heap
// Can still be updated using the update sub resource API
struct D3D12StaticBuffer : public D3D12RenderResource
{
	PlacedGPUResource gpu_placed_resource;
	union {
		struct _Vertex_Index {
			D3D12_VERTEX_BUFFER_VIEW vbv;
			D3D12_INDEX_BUFFER_VIEW ibv;
		} view;
		union {
			OfflineDescriptorHeapHandle cbv;
			struct _SRV_UAV {
				OfflineDescriptorHeapHandle srv;
				OfflineDescriptorHeapHandle uav;
			} srv_uav;
		};
	};
	unsigned aligned_size;
	unsigned size;
};

}
