#pragma once

#include <d3d11.h>
#include "render_atom.h"
#include <core/handle/handle_container.h>


namespace ue
{

enum class RenderResourceType : unsigned
{
	DepthStencilState,
	BlendState,
	RasterizerState,

	VSShader,
	PSShader,
	GSShader,
	DSShader,
	HSShader,
	CSShader,

	Textures,
	RenderTargets,
	Samplers,
	ConstantBuffers,
	Buffers,

	InputLayouts,
	IndexBuffers,
	VertexBuffers,

	RenderResourceCount,
};

struct RenderResource
{
	RenderResourceType type;
	unsigned handle;
	unsigned hash;
};

class D3D11ResourceManager
{
public:



	void get_depth_stencil_state(Handle handle, ID3D11DepthStencilState** depth_stencil_state, unsigned* state_hash);
	void get_blend_state(Handle handle, ID3D11BlendState** blend_state);
	void get_rasterizer_state(Handle handle, ID3D11RasterizerState** rasterizer_state);
	void get_vertex_buffer(Handle handle, ID3D11Buffer** vertex_buffer);


	Handle create_render_atom(Handle* resource_handles, unsigned resource_count);
	D3D11RenderAtom* get_render_atom(Handle atom_handle);


private:

	HandleContainer<8192> _resources;
	HandleContainer<4096> _render_atoms;
};


}