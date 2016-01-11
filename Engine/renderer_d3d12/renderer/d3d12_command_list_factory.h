#pragma once

#include <core/platform/platform.h>
#include <renderer_d3d12/stdinclude.h>
#include <vector>
#include <queue>

namespace ue
{

struct D3D12GraphicsCommandList
{
	ID3D12GraphicsCommandList* operator()() { return command_list.Get(); }
	ID3D12CommandList * const* command_list_cast() { return reinterpret_cast<ID3D12CommandList*const*>(reinterpret_cast<ID3D12GraphicsCommandList*const*>(command_list.Get())); }
	

	
	ComPtr<ID3D12GraphicsCommandList> command_list;

	//ComPtr<ID3D12Fence> _fence;
	//uint64_t _fence_value;
	unsigned index;
};

typedef std::vector<D3D12GraphicsCommandList*> D3D12GraphicsCommandLists;
typedef std::queue<unsigned> D3D12GraphicsCommandListFreeList;
typedef ComPtr<ID3D12CommandAllocator> D3D12GraphicsCommandAllocator;

struct D3D12GraphicsThreadData
{
	D3D12GraphicsCommandAllocator* _graphics_command_allocators;
	D3D12GraphicsCommandLists _graphics_command_lists;
	D3D12GraphicsCommandListFreeList _graphics_command_list_free_list;
};

class D3D12RenderDevice;

class D3D12CommandListFactory
{
public:
	D3D12CommandListFactory(D3D12RenderDevice& render_device);
	~D3D12CommandListFactory();

	static void thread_initialize(D3D12CommandListFactory* command_list_factory);

	D3D12GraphicsCommandList& get_graphics_command_list(unsigned frame_index, ID3D12PipelineState* pipeline_state = nullptr);
	void executed(const D3D12GraphicsCommandList& command_list);
	void reset_command_allocators(unsigned frame_index);

	void reset(D3D12GraphicsCommandList* command_list, ID3D12PipelineState* pipeline_state = nullptr);

private:	
	D3D12RenderDevice& _render_device;

	void reset_command_list(ID3D12GraphicsCommandList* command_list, ID3D12CommandAllocator* allocator, ID3D12PipelineState* pipeline_state);
	ComPtr<ID3D12GraphicsCommandList> create_graphics_command_list(ID3D12CommandAllocator* allocator, D3D12_COMMAND_LIST_TYPE type, ID3D12PipelineState* pipeline_state);

	static UE_THREAD_LOCAL D3D12GraphicsThreadData* _tls_graphics_thread_data;
	std::vector<D3D12GraphicsThreadData*> _graphics_thread_data;
};

}