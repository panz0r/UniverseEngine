#pragma once

#include <d3d12.h>
#include <core/platform/platform.h>

#include <renderer_d3d12/renderer/command_list.h>
#include <renderer_d3d12/renderer/render_device.h>
#include <renderer_d3d12/com/com_ptr.h>
#include <vector>
#include <queue>
#include <atomic>

namespace ue
{

class D3D12RenderDevice;


typedef std::vector<CommandListInterface<ID3D12GraphicsCommandList>* > GraphicsCommandLists;
typedef std::queue<CommandListInterface<ID3D12GraphicsCommandList>* > CommandListFreeList;

class CommandListFactory
{
public:
	enum CommandListType { Graphics, Compute };
	CommandListFactory(D3D12RenderDevice &render_device);
	~CommandListFactory();

	static void thread_initialize();
	
	template <class T>
	inline CommandListInterface<T> *aquire_command_list();

	template <class T>
	void release_command_list(CommandListInterface<T> *command_list);

private:
	D3D12RenderDevice &_render_device;
	
	static __THREAD_LOCAL GraphicsCommandLists *_graphics_command_lists;
	static __THREAD_LOCAL CommandListFreeList *_graphics_command_lists_free;
};

template <>
inline CommandListInterface<ID3D12GraphicsCommandList> *CommandListFactory::aquire_command_list()
{
	// Not thread safe in this current state

	ID3D12Device * device = _render_device.device();
	
	if (!_graphics_command_lists_free->empty()) {
		auto ret = _graphics_command_lists_free->front();
		if (ret->_state.load(std::memory_order_acquire) == Submitted) {
			if (ret->_fence->GetCompletedValue() >= ret->_fence_value) {
				_graphics_command_lists_free->pop();
				ret->_command_allocator->Reset();
				ret->_command_list->Reset(ret->_command_allocator.Get(), nullptr);
				return ret;
			}
		}
	}

	CommandListInterface<ID3D12GraphicsCommandList> *command_list_storage = new CommandListInterface<ID3D12GraphicsCommandList>();
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_list_storage->_command_allocator));
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, command_list_storage->_command_allocator.Get(), nullptr, IID_PPV_ARGS(&command_list_storage->_command_list));
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&command_list_storage->_fence));

	_graphics_command_lists->push_back(command_list_storage);
	return command_list_storage;
}

template <>
inline void CommandListFactory::release_command_list(CommandListInterface<ID3D12GraphicsCommandList> *command_list_interface)
{
	command_list_interface->_state.store(WaitingToSubmit, std::memory_order_release);
	_graphics_command_lists_free->push(command_list_interface);
}

}