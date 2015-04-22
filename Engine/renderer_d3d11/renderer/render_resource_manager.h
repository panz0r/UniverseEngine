#pragma once

#include <d3d11.h>
#include "compiled_drawcall.h"

namespace ue
{

enum class ResourceType : unsigned
{
	DepthStencilState,
	BlendState,
	RasterizerState,

	VSShader,
	VSSamplers,
	VSResources,
	VSCBuffers,

	PSShader,
	PSSamplers,
	PSResources,
	PSCBuffers,

	InputLayout,
	IndexBuffer,
	VertexBuffers,

	GSShader,
	GSSamplers,
	GSResources,
	GSCBuffers,

	DSShader,
	DSSamplers,
	DSResources,
	DSCBuffers,

	HSShader,
	HSSamplers,
	HSResources,
	HSCBuffers,

};

struct RenderResource
{
	ResourceType type;
	unsigned handle;
	unsigned hash;
};

class D3D11ResourceManager
{
public:

	void get_depth_stencil_state(unsigned handle, ID3D11DepthStencilState** depth_stencil_state, unsigned* state_hash);
	void get_blend_state(unsigned handle, ID3D11BlendState** blend_state);
	void get_rasterizer_state(unsigned handle, ID3D11RasterizerState** rasterizer_state);
	void get_vertex_buffer(unsigned handle, ID3D11Buffer** vertex_buffer);

	D3D11CompiledDrawCall* get_cached_drawcall(unsigned drawcall_hash);

};


}