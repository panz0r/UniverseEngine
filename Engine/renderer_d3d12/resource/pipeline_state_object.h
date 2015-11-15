#pragma once
#include "common.h"

namespace ue
{




struct PipelineStateObjectDesc
{
	RenderHandle input_layout;
	RenderHandle root_signature;
	RenderHandle shaders[ShaderCount];
	RenderHandle rasterizer_state;
	RenderHandle blend_state;
	RenderHandle depth_stencil_state;

	unsigned sample_mask;
	unsigned primitive_topology_type;
	unsigned rtv_formats[8];
	unsigned dvs_format;
	unsigned num_render_targets;
	struct _SampleDesc
	{
		unsigned count;
		unsigned quality;
	} sample_desc;
};


struct PipelineStateResource : public RenderResource
{
	ComPtr<ID3D12PipelineState> _pipeline_state;
};

}