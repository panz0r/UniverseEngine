#include "d3d12_command_list_factory.h"
#include <renderer_d3d12/renderer/d3d12_render_device.h>

namespace ue
{

UE_THREAD_LOCAL D3D12GraphicsThreadData* D3D12CommandListFactory::_tls_graphics_thread_data = nullptr;

D3D12CommandListFactory::D3D12CommandListFactory(D3D12RenderDevice& render_device)
	: _render_device(render_device)
{
}

/////////////////////////////////////////////////////////

D3D12CommandListFactory::~D3D12CommandListFactory()
{}

/////////////////////////////////////////////////////////

void D3D12CommandListFactory::thread_initialize(D3D12CommandListFactory* command_list_factory)
{
	D3D12RenderDevice& render_device = command_list_factory->_render_device;
	ID3D12Device* device = command_list_factory->_render_device.device();
	
	_tls_graphics_thread_data = new D3D12GraphicsThreadData();
	_tls_graphics_thread_data->_graphics_command_allocators = new D3D12GraphicsCommandAllocator[render_device.back_buffer_count()];
	for (unsigned i = 0; i < render_device.back_buffer_count(); ++i) {
		D3D12GraphicsCommandAllocator& command_allocator = _tls_graphics_thread_data->_graphics_command_allocators[i];
		device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocator));
	}
	command_list_factory->_graphics_thread_data.push_back(_tls_graphics_thread_data);
}

/////////////////////////////////////////////////////////

D3D12GraphicsCommandList& D3D12CommandListFactory::get_graphics_command_list(unsigned frame_index, ID3D12PipelineState* pipeline_state)
{
	if(_tls_graphics_thread_data == nullptr)
		D3D12CommandListFactory::thread_initialize(this);

	D3D12GraphicsCommandLists& command_lists = _tls_graphics_thread_data->_graphics_command_lists;
	D3D12GraphicsCommandListFreeList& free_list = _tls_graphics_thread_data->_graphics_command_list_free_list;
	D3D12GraphicsCommandAllocator* allocators = _tls_graphics_thread_data->_graphics_command_allocators;

	if (!free_list.empty()) {
		unsigned index = free_list.front();
		D3D12GraphicsCommandList* command_list = command_lists[index];
		reset_command_list(command_list->command_list.Get(), allocators[frame_index].Get(), pipeline_state);
		free_list.pop();
		return *command_list;
	}

	D3D12GraphicsCommandList* command_list = new D3D12GraphicsCommandList();
	command_list->index = (unsigned)command_lists.size();
	command_list->command_list = create_graphics_command_list(allocators[frame_index].Get(), D3D12_COMMAND_LIST_TYPE_DIRECT, pipeline_state);

	command_lists.push_back(command_list);
	return *command_list;
}

/////////////////////////////////////////////////////////

void D3D12CommandListFactory::executed(const D3D12GraphicsCommandList& command_list) 
{
	D3D12GraphicsCommandListFreeList& free_list = _tls_graphics_thread_data->_graphics_command_list_free_list;
	free_list.push(command_list.index);
}

/////////////////////////////////////////////////////////

void D3D12CommandListFactory::reset_command_allocators(unsigned frame_index)
{
	for (auto graphics_thread_data : _graphics_thread_data) {
		HRESULT hr = graphics_thread_data->_graphics_command_allocators[frame_index].Get()->Reset();
		int apa = 1;
	}
}

/////////////////////////////////////////////////////////

void D3D12CommandListFactory::reset_command_list(ID3D12GraphicsCommandList* command_list, ID3D12CommandAllocator* allocator, ID3D12PipelineState* pipeline_state)
{
	command_list->Reset(allocator, pipeline_state);
}

/////////////////////////////////////////////////////////

void D3D12CommandListFactory::reset(D3D12GraphicsCommandList* command_list, ID3D12PipelineState* pipeline_state)
{
	(*command_list)()->Reset(_tls_graphics_thread_data->_graphics_command_allocators[command_list->index].Get(), pipeline_state);
}

/////////////////////////////////////////////////////////

ComPtr<ID3D12GraphicsCommandList> D3D12CommandListFactory::create_graphics_command_list(ID3D12CommandAllocator* allocator, D3D12_COMMAND_LIST_TYPE type, ID3D12PipelineState* pipeline_state)
{

	ID3D12Device* device = _render_device.device();
	ComPtr<ID3D12GraphicsCommandList> command_list;
	device->CreateCommandList(0, type, allocator, pipeline_state, IID_PPV_ARGS(&command_list));
	return command_list;
}


}