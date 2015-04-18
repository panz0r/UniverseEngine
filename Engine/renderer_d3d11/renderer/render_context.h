#pragma once

#include <d3d11.h>

namespace ue
{


typedef unsigned statemask_t;


enum DrawCallStates
{
	DepthStencilState,
	BlendState,
	RasterizerState,
	ViewPort,
	Scissors,

	VSShader,
	VSSamplers,
	VSResources,
	VSCBuffers,

	PSShader,
	PSSamplers,
	PSResources,
	PSCBuffers,

	InputLayout,
	PrimitiveTopology,
	IndexBuffer,
	VertexBuffers,

	// Extended states
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

	DrawCallStatesCount,

};


class D3D11RenderContext
{
public:
	D3D11RenderContext(ID3D11DeviceContext* context);
	~D3D11RenderContext();
	
	void drawcall(statemask_t dirty_mask);
	void compute();

private:
	statemask_t compute_drawcall_dirty_mask(const unsigned* drawcall_states);

	unsigned _drawcall_states[DrawCallStatesCount];
	ID3D11DeviceContext* _context;
};


}