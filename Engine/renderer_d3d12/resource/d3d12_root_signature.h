#pragma once

#include <renderer_d3d12/stdinclude.h>

namespace ue
{

typedef ComPtr<ID3D12RootSignatureDeserializer> D3D12DeserializedRootSignature;

struct D3D12SerializedRootSignature
{
	unsigned hash;
	ComPtr<ID3DBlob> blob;
};

struct D3D12RootSignature
{
	ComPtr<ID3D12RootSignature> signature;
};


}