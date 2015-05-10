#pragma once

namespace ue
{



enum DrawCallStates
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
	PrimitiveTopology,
	IndexBuffer,
	VertexBuffers,

	DrawCallStateCount,

	// Extended states
	GSShader = DrawCallStateCount,
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

	ExtendedDrawCallStateCount,
};


}