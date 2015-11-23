#pragma once

#include <d3d12.h>
#include <renderer_d3d12/com/com_ptr.h>
#include <renderer_d3d12/renderer/d3dx12.h>
#include <core/handle/handle_container.h>

namespace ue
{

enum ShaderType
{
	VertexShader,
	PixelShader,
	GeometryShader,
	HullShader,
	DomainShader,
	ComputeShader,

	ShaderCount
};

struct RenderResource
{
	enum ResourceType { 
		Texture, 
		RenderTarget, 
		Buffer, 
		ConstantBuffer, 
		PipelineState, 
		InstancedRenderAtom 
	};

	ResourceType type;

	virtual D3D12_GPU_VIRTUAL_ADDRESS gpu_virtual_address() { return D3D12_GPU_VIRTUAL_ADDRESS(); }
	virtual D3D12_CPU_DESCRIPTOR_HANDLE cpu_descriptor_handle() { return D3D12_CPU_DESCRIPTOR_HANDLE(); };
};

typedef Handle RenderHandle;


}