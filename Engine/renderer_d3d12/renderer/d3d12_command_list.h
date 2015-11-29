#pragma once

#include <d3d12.h>
#include <vector>
#include "../com/com_ptr.h"
#include <core/platform/platform.h>

namespace ue
{

struct D3D12CommandList;

class D3D12CommandListFactory
{
public:

	D3D12CommandList * new_graphics_command_list();
	D3D12CommandList * new_compute_command_list();

private:
	
	static __THREAD_LOCAL ComPtr<ID3D12GraphicsCommandList> _graphics_command_list;
	static __THREAD_LOCAL ComPtr<ID3D12CommandAllocator> * _frame_command_allocator;
};


}