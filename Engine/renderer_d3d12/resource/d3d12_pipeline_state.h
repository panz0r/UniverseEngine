#pragma once

#include <renderer_d3d12/stdinclude.h>
#include "d3d12_shader_factory.h"
#include "d3d12_render_target.h"

namespace ue
{


struct D3D12GraphicsPipelineStateDesc
{
	// TEMP
	D3D12_INPUT_LAYOUT_DESC input_layout;
	D3D12Shader shaders[GraphicsShaderTypeCount];
	D3D12RootSignature root_signature;
	unsigned primitive_topology_type;
	unsigned render_target_count;
	unsigned render_target_formats[8];
	unsigned depth_stencil_format;
};

struct D3D12PipelineState
{
	ComPtr<ID3D12PipelineState> pipeline_state;
	D3D12RootSignature root_signature;
};


}