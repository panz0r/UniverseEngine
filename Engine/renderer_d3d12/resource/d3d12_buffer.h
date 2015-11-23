#pragma once

#include "d3d12_render_resource.h"
#include "placed_gpu_resource.h"

namespace ue
{

struct D3D12Buffer : public D3D12RenderResource
{
	static const D3D12RenderResource::Type Type = D3D12RenderResource::Buffer;

	PlacedGPUResource placed_resource;
	OfflineDescriptorHeapHandle offline_descriptor_handle;
	D3D12_CPU_DESCRIPTOR_HANDLE srv_descriptor_handle;
	D3D12_CPU_DESCRIPTOR_HANDLE uav_descriptor_handle;
	union
	{
		D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view;
		D3D12_INDEX_BUFFER_VIEW index_buffer_view;
	};
	void * data;

	unsigned aligned_size;
	unsigned size;
};

}
