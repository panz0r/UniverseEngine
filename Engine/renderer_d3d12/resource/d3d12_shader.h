#pragma once

#include <renderer_d3d12/stdinclude.h>

namespace ue
{

enum ShaderType
{
	VertexShader,
	PixelShader,
	GraphicsShaderTypeCount,
	
	ComputeShader
};

struct D3D12Shader
{
	ComPtr<ID3DBlob> blob;
};

}