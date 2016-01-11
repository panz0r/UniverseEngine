#pragma once

#include <renderer_d3d12/stdinclude.h>
#include "d3d12_shader.h"
#include "d3d12_root_signature.h"

namespace ue
{

class D3D12RenderDevice;

class D3D12ShaderFactory
{
public:
	D3D12ShaderFactory(D3D12RenderDevice& render_device);
	~D3D12ShaderFactory();

	D3D12Shader compile_shader_from_source(const char* source, const char* entry, const char* target, unsigned flags);
	D3D12SerializedRootSignature compile_root_signature_from_source(const char* source, const char* rs_name, unsigned flags);

	D3D12DeserializedRootSignature deserialize_root_signature(ID3DBlob* blob);
	D3D12SerializedRootSignature serialize_root_signature(const D3D12DeserializedRootSignature& deserialized_root_signature);
	D3D12SerializedRootSignature serialize_root_signature(const D3D12_ROOT_SIGNATURE_DESC& desc);

	
private:

	unsigned hash_root_signature(ID3DBlob* blob);

	D3D12RenderDevice& _render_device;
};

}