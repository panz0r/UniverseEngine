#include "d3d12_shader_factory.h"
#include <renderer_d3d12/renderer/d3d12_render_device.h>
#include <d3dcompiler.h>
#include <core/assert/assert.h>

#include <math/hash.h>

namespace ue
{

D3D12ShaderFactory::D3D12ShaderFactory(D3D12RenderDevice& render_device)
	: _render_device(render_device)
{
}

/////////////////////////////////////////////////////////

D3D12ShaderFactory::~D3D12ShaderFactory()
{
}

/////////////////////////////////////////////////////////

D3D12Shader D3D12ShaderFactory::compile_shader_from_source(const char* source, const char* entry, const char* target, unsigned flags)
{
	size_t source_size = strlen(source);
	
	ComPtr<ID3DBlob> shader_blob;
	ComPtr<ID3DBlob> error;
	HRESULT hr = D3DCompile(source, source_size, "custom", nullptr, nullptr, entry, target, flags, 0, &shader_blob, &error);
	
	
	if (FAILED(hr))
	{
		OutputDebugStringA((const char*)error->GetBufferPointer());
		return D3D12Shader();
	}

	D3D12Shader shader = { shader_blob };
	return shader;
}

/////////////////////////////////////////////////////////

D3D12SerializedRootSignature D3D12ShaderFactory::compile_root_signature_from_source(const char* source, const char* rs_name, unsigned flags)
{
	size_t source_size = strlen(source);

	ComPtr<ID3DBlob> blob;
	ComPtr<ID3DBlob> error;
	HRESULT hr = D3DCompile(source, source_size, "root_signature", nullptr, nullptr, rs_name, "rootsig_1_0", flags, 0, &blob, &error);

	if (FAILED(hr))
	{
		OutputDebugStringA((const char*)error->GetBufferPointer());
		return D3D12SerializedRootSignature();
	}

	D3D12SerializedRootSignature serialized_root_signature = { hash_root_signature(blob.Get()), blob };
	return serialized_root_signature;
}

/////////////////////////////////////////////////////////

D3D12DeserializedRootSignature D3D12ShaderFactory::deserialize_root_signature(ID3DBlob* blob)
{
	ComPtr<ID3D12RootSignatureDeserializer> root_signature_deserializer;
	HRESULT hr = D3D12CreateRootSignatureDeserializer(blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&root_signature_deserializer));
	if(FAILED(hr))
		return D3D12DeserializedRootSignature();

	return root_signature_deserializer;
}

/////////////////////////////////////////////////////////

D3D12SerializedRootSignature D3D12ShaderFactory::serialize_root_signature(const D3D12DeserializedRootSignature& deserialized_root_signature)
{
	const D3D12_ROOT_SIGNATURE_DESC* root_signature_desc = deserialized_root_signature->GetRootSignatureDesc();

	ComPtr<ID3DBlob> root_signature_blob;
	ComPtr<ID3DBlob> error;
	HRESULT hr = D3D12SerializeRootSignature(root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &root_signature_blob, &error);
	if (FAILED(hr)) {
		OutputDebugStringA((const char*)error->GetBufferPointer());
		return D3D12SerializedRootSignature();
	}

	unsigned hash = hash_root_signature(root_signature_blob.Get());
	D3D12SerializedRootSignature serialized_root_signature = { hash, root_signature_blob };
	return serialized_root_signature;
}

/////////////////////////////////////////////////////////

D3D12SerializedRootSignature D3D12ShaderFactory::serialize_root_signature(const D3D12_ROOT_SIGNATURE_DESC& desc)
{
	ComPtr<ID3DBlob> root_signature_blob;
	ComPtr<ID3DBlob> error;
	HRESULT hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &root_signature_blob, &error);
	if (FAILED(hr)) {
		OutputDebugStringA((const char*)error->GetBufferPointer());
		return D3D12SerializedRootSignature();
	}

	unsigned hash = hash_root_signature(root_signature_blob.Get());
	D3D12SerializedRootSignature serialized_root_signature = { hash, root_signature_blob };
	return serialized_root_signature;
}

/////////////////////////////////////////////////////////

unsigned D3D12ShaderFactory::hash_root_signature(ID3DBlob* blob)
{
	return default_hash(blob->GetBufferPointer(), (unsigned)blob->GetBufferSize(), 0);
}

/////////////////////////////////////////////////////////

} // namespace ue