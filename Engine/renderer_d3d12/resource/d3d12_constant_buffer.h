#pragma once

#include "d3d12_render_resource.h"

namespace ue
{

struct D3D12ConstantBuffer : public D3D12RenderResource
{
	static const D3D12RenderResource::Type Type = D3D12RenderResource::ConstantBuffer;

	OfflineDescriptorHeapHandle offline_descriptor_handle;
	D3D12_CPU_DESCRIPTOR_HANDLE cbv_descriptor_handle;
	D3D12_GPU_VIRTUAL_ADDRESS gpu_virtual_address;

	void * data;
	unsigned aligned_size;
	unsigned size;
	
	inline D3D12_CPU_DESCRIPTOR_HANDLE get_cpu_descriptor_handle(unsigned index = 0);
	inline D3D12_GPU_VIRTUAL_ADDRESS get_gpu_virtual_address(unsigned index = 0);
	inline void * data_ptr(unsigned index = 0);
};

//////////////////////////////////////////////

inline D3D12_CPU_DESCRIPTOR_HANDLE D3D12ConstantBuffer::get_cpu_descriptor_handle(unsigned index = 0)
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(cbv_descriptor_handle, index, __cbv_srv_uav_descriptor_increment_size);
}

//////////////////////////////////////////////

inline D3D12_GPU_VIRTUAL_ADDRESS D3D12ConstantBuffer::get_gpu_virtual_address(unsigned index = 0)
{
	return gpu_virtual_address + (index * aligned_size);
}

//////////////////////////////////////////////

inline void * D3D12ConstantBuffer::data_ptr(unsigned index = 0)
{
	return reinterpret_cast<void*>(reinterpret_cast<size_t>(data) + (index * aligned_size));
}


}
