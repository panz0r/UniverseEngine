#pragma once

#include "common.h"
#include "placed_gpu_resource.h"
#include <renderer_d3d12/renderer/offline_descriptor_heap.h>

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
	static const ResourceType Type = RenderResource::Texture;

	PlacedGPUResource placed_resource;

	OfflineDescriptorHeapHandle *handles;
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
	bool depth_stencil;
};

struct RenderTargetResource : public RenderResource
{
	static const ResourceType Type = RenderResource::RenderTarget;

	enum Binding { 
		PRESENT = 0x0, 
		RENDER_TARGET = D3D12_RESOURCE_STATE_RENDER_TARGET, 
		DEPTH_WRITE = D3D12_RESOURCE_STATE_DEPTH_WRITE,
		SHADER_RESOURCE = (unsigned)D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | (unsigned)D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE 
	};

	ComPtr<ID3D12Resource> resource;
	OfflineDescriptorHeapHandle *handle;
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