#pragma once

#include "common.h"
#include "placed_gpu_resource.h"
#include <renderer_d3d12/renderer/descriptor_heap.h>

namespace ue
{

struct TextureDesc
{
	unsigned width;
	unsigned height;
	unsigned depth;
	unsigned mip_levels;
	unsigned format;
	unsigned dimension;
	bool dynamic;
};

struct TextureResource : public RenderResource
{
	PlacedGPUResource placed_resource;

	DescriptorHeapHandle *handles;
	D3D12_CPU_DESCRIPTOR_HANDLE *srv;
	D3D12_CPU_DESCRIPTOR_HANDLE *uav;

	void *mapped_data;
	size_t size;
	bool dynamic;
};


struct RenderTargetDesc
{
	unsigned width;
	unsigned height;
	unsigned format;
};

struct RenderTargetResource : public RenderResource
{
	enum Binding { PRESENT = 0x0, RENDER_TARGET = 0x4, PIXEL_SHADER_RESOURCE = 0x80 };
	ComPtr<ID3D12Resource> resource;
	DescriptorHeapHandle *handle;
	D3D12_CPU_DESCRIPTOR_HANDLE *srv;
	union {
		D3D12_CPU_DESCRIPTOR_HANDLE *rtv;
		D3D12_CPU_DESCRIPTOR_HANDLE *dsv;
	};
	Binding current_state;

	void transition(ID3D12GraphicsCommandList *command_list, Binding binding)
	{
		command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(), (D3D12_RESOURCE_STATES)current_state, (D3D12_RESOURCE_STATES)binding));
		current_state = binding;
	}

	// srv
	// rtv/dsv
	// current state
};

struct Sampler
{
	// sampler view
	D3D12_CPU_DESCRIPTOR_HANDLE handle;
};



}