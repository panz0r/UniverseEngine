#pragma once

#include <renderer_d3d12/memory/d3d12_allocation.h>

#include "d3d12_resource.h"
#include "d3d12_render_resource.h"

namespace ue
{

struct D3D12BufferDesc
{
	unsigned size;
	union {
		unsigned stride;
		unsigned format;
	};
	bool dynamic;
};

struct D3D12Buffer
{
	bool dynamic;
	D3D12Allocation allocation;
	D3D12Resource resource;
	unsigned size;
	unsigned alignment;
};

struct D3D12RawBuffer : public D3D12RenderResource
{
	D3D12Buffer buffer;
	D3D12DescriptorAllocation srv_allocation;
	D3D12DescriptorAllocation uav_allocation;
};

struct D3D12ConstantBuffer : public D3D12RenderResource
{
	D3D12Buffer buffer;
	D3D12DescriptorAllocation cbv_allocation;
};

struct D3D12VertexBufferDesc
{
	unsigned size;
	unsigned stride;
};

struct D3D12VertexBuffer : public D3D12RenderResource
{
	D3D12Buffer buffer;
	D3D12_VERTEX_BUFFER_VIEW vbv;
};

struct D3D12IndexBufferDesc
{
	unsigned size;
	unsigned format;
};

struct D3D12IndexBuffer : public D3D12RenderResource
{
	D3D12Buffer buffer;
	D3D12_INDEX_BUFFER_VIEW ibv;
};

}
