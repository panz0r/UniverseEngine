#pragma once

#include "d3d12_render_resource.h"
#include "d3d12_placed_resource.h"

namespace ue
{

struct D3D12TextureDesc
{
	unsigned width;
	unsigned height;
	unsigned depth;
	unsigned mip_levels;
	unsigned format;
	unsigned dimension;
	bool dynamic;
};

struct D3D12Texture : public D3D12RenderResource
{
	D3D12PlacedResource placed_resource;
	D3D12DescriptorAllocation srv_allocation;
	D3D12DescriptorAllocation uav_allocation;
	void * data;
	unsigned aligned_size;
	unsigned size;
};



}