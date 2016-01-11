#pragma once

#include <renderer_d3d12/stdinclude.h>

namespace ue
{

struct D3D12RenderTargetDesc
{
	unsigned width;
	unsigned height;
	unsigned format;
	bool depth_stencil;
};


struct D3D12RenderTarget : public D3D12RenderResource
{
	enum Descriptor { SRV_DESCRIPTOR, UAV_DESCRIPTOR, RTV_DESCRIPTOR, DSV_DESCRIPTOR };
	ComPtr<ID3D12Resource> resource;
	D3D12DescriptorAllocation srv_allocation;
	D3D12DescriptorAllocation uav_allocation;
	union {
		D3D12DescriptorAllocation rtv_allocation;
		D3D12DescriptorAllocation dsv_allocation;
	};
	unsigned resource_state;
	unsigned format;
	unsigned aligned_size;
	unsigned size;
};

}