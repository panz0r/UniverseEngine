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
	enum ResourceType { Texture, RenderTarget, Buffer, PipelineState, InstancedRenderAtom };
	ResourceType type;
};

typedef Handle RenderHandle;


}