#pragma once

#include <renderer_d3d12/stdinclude.h>

namespace ue
{

struct D3D12Resource
{
	ComPtr<ID3D12Resource> resource;
	D3D12_RESOURCE_STATES resource_state;
};

}