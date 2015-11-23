#pragma once

#include <d3d12.h>
#include "../renderer/offline_descriptor_heap.h"

namespace ue
{

extern unsigned __cbv_srv_uav_descriptor_increment_size;
extern unsigned __sampler_descriptor_increment_size;

struct D3D12RenderResource
{
	enum Type {
		Texture,
		RenderTarget,
		Buffer,
		ConstantBuffer,
		PipelineState,
		InstancedRenderAtom
	};
	Type type;

	//virtual D3D12_GPU_VIRTUAL_ADDRESS gpu_virtual_address() { return D3D12_GPU_VIRTUAL_ADDRESS(); }
	//virtual D3D12_CPU_DESCRIPTOR_HANDLE cpu_descriptor_handle() { return D3D12_CPU_DESCRIPTOR_HANDLE(); };
};


}
