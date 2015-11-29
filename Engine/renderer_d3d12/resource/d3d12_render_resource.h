#pragma once

#include <d3d12.h>
#include "../renderer/offline_descriptor_heap.h"

namespace ue
{

extern unsigned __cbv_srv_uav_descriptor_increment_size;
extern unsigned __sampler_descriptor_increment_size;


enum ResourceType {

	VertexBuffer,
	IndexBuffer,
	ConstantBuffer,
	RawBuffer,

	Texture1D,
	Texture2D,
	Texture3D,
	TextureCube,

	RenderTarget,
	DepthStencilTarget,
};

struct D3D12RenderResource
{

	ResourceType type;
	D3D12_RESOURCE_STATES resource_state;

	//inline virtual D3D12_CPU_DESCRIPTOR_HANDLE get_cbv_descriptor_handle(unsigned index = 0) { return D3D12_CPU_DESCRIPTOR_HANDLE(); }
	//inline virtual D3D12_CPU_DESCRIPTOR_HANDLE get_srv_descriptor_handle(unsigned index = 0) { return D3D12_CPU_DESCRIPTOR_HANDLE(); }
	//inline virtual D3D12_CPU_DESCRIPTOR_HANDLE get_uav_descriptor_handle(unsigned index = 0) { return D3D12_CPU_DESCRIPTOR_HANDLE(); }
	//inline virtual D3D12_GPU_VIRTUAL_ADDRESS get_gpu_virtual_address(unsigned index = 0) { return D3D12_GPU_VIRTUAL_ADDRESS(); }
	//inline virtual void * access_data(unsigned index = 0) { return nullptr; }
};


}
