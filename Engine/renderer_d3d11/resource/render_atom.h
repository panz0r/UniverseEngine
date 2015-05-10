#pragma once

#include <d3d11.h>
#include <renderer_d3d11/renderer/drawcall_states.h>


namespace ue
{

static size_t s_nullarray[16] = { 0 };

enum class DrawCallType : unsigned
{
	Indexed,
	Instanced,
};

struct D3D11RenderAtom
{
	enum Offsets
	{
		DepthStencilState,
		StencilRef,
		BlendState,
		BlendFactor,
		SampleMask,
		RasterizerState,

		VertexShader,
		VSSamplerStart,
		VSSamplerCount,
		VSSamplers,
		VSResourceStart,
		VSResourceCount,
		VSResources,
		VSCBStart,
		VSCBCount,
		VSConstantBuffers,

		PixelShader,
		PSSamplerStart,
		PSSamplerCount,
		PSSamplers,
		PSResourceStart,
		PSResourceCount,
		PSResources,
		PSCBStart,
		PSCBCount,
		PSConstantBuffers,

		InputLayout,

		IndexBuffer,
		IndexFormat,
		IndexOffset,

		VBStart,
		VBCount,
		VBuffers,
		VBStrides,
		VBOffsets,

		OffsetCount
	};

	inline void* ptr(unsigned type);
	inline void** ptrptr(unsigned type);
	inline unsigned uint(unsigned type);

	// Always available
	D3D11_PRIMITIVE_TOPOLOGY	primitive_topology;
	DrawCallType				draw_call_type;
	unsigned					index_count;
	unsigned					start_index_location;
	int							base_vertex_location;
	unsigned					instance_count;
	unsigned					start_instance_location;

	unsigned					state_hashes[DrawCallStateCount];
	unsigned short				offsets[OffsetCount];
	char						data[1];
};


inline void* D3D11RenderAtom::ptr(unsigned type)
{
	return ((offsets[type] == (unsigned short)-1) ? (void*)&s_nullarray[0] : (void*)(data + offsets[type]));
}

inline void** D3D11RenderAtom::ptrptr(unsigned type)
{
	return ((offsets[type] == (unsigned short)-1) ? (void**)&s_nullarray[0] : (void**)(data + offsets[type]));
}

inline unsigned D3D11RenderAtom::uint(unsigned type)
{
	return ((offsets[type] == (unsigned short)-1) ? (unsigned)s_nullarray[0] : *(unsigned*)(data + offsets[type]));
}

}
