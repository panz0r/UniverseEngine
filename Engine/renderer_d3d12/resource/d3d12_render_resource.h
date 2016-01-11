#pragma once

#include <d3d12.h>
#include "../renderer/offline_descriptor_heap.h"

namespace ue
{

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
};


}
