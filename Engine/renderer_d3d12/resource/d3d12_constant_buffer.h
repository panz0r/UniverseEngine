#pragma once

#include "d3d12_render_resource.h"

namespace ue
{

struct D3D12ConstantBuffer : public D3D12RenderResource
{

	OfflineDescriptorHeapHandle cbv_descriptor_handle;
	D3D12_GPU_VIRTUAL_ADDRESS gpu_virtual_address;

	void * data;
	unsigned aligned_size;
	unsigned size;
	
	inline D3D12_GPU_VIRTUAL_ADDRESS get_gpu_virtual_address(unsigned index = 0);
	inline void * access_data(unsigned index = 0);
};


//////////////////////////////////////////////

inline D3D12_GPU_VIRTUAL_ADDRESS D3D12ConstantBuffer::get_gpu_virtual_address(unsigned index)
{
	return gpu_virtual_address + (index * aligned_size);
}

//////////////////////////////////////////////

inline void * D3D12ConstantBuffer::access_data(unsigned index)
{
	return reinterpret_cast<void*>(reinterpret_cast<size_t>(data) + (index * aligned_size));
}


}
