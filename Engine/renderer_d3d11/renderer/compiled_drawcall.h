#pragma once

namespace ue
{

enum class DrawCallType : unsigned
{
	Indexed,
	Instanced,
};

enum DrawCallStates
{
	DepthStencilState,
	BlendState,
	RasterizerState,
	//ViewPort,
	//Scissors,

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

struct D3D11CompiledDrawCall
{
	ID3D11DepthStencilState*	depth_stencil_state;
	unsigned*					stencil_ref;
	ID3D11BlendState*			blend_state;
	float*						blend_factor;
	unsigned*					sample_mask;
	ID3D11RasterizerState*		rasterizer_state;

	ID3D11VertexShader*			vertex_shader;
	unsigned*					vs_samplers_start_slot;
	unsigned*					vs_samplers_count;
	ID3D11SamplerState**		vs_samplers;

	unsigned*					vs_resource_start_slot;
	unsigned*					vs_resource_count;
	ID3D11ShaderResourceView**	vs_shader_resource_views;

	unsigned*					vs_cb_start_slot;
	unsigned*					vs_cb_count;
	ID3D11Buffer**				vs_constant_buffers;

	ID3D11PixelShader*			pixel_shader;
	unsigned*					ps_samplers_start_slot;
	unsigned*					ps_samplers_count;
	ID3D11SamplerState**		ps_samplers;

	unsigned*					ps_resource_start_slot;
	unsigned*					ps_resource_count;
	ID3D11ShaderResourceView**	ps_shader_resource_views;

	unsigned*					ps_cb_start_slot;
	unsigned*					ps_cb_count;
	ID3D11Buffer**				ps_constant_buffers;


	ID3D11InputLayout*			input_layout;
	

	ID3D11Buffer*				index_buffer;
	DXGI_FORMAT*				index_format;
	unsigned*					index_offset;

	unsigned*					vb_start_slot;
	unsigned*					vb_count;
	ID3D11Buffer**				vb_buffers;
	unsigned*					vb_strides;
	unsigned*					vb_offsets;

	D3D11_PRIMITIVE_TOPOLOGY	primitive_topology;
	DrawCallType				draw_call_type;
	unsigned					index_count;
	unsigned					start_index_location;
	int							base_vertex_location;

	unsigned					instance_count;
	unsigned					start_instance_location;
	
	char						data_blob[1];
};

}