#pragma once

#include <core/platform/platform.h>
#include <renderer_d3d12/com/com_ptr.h>
#include <d3d12.h>
#include <vector>
#include <queue>
#include <atomic>

namespace ue
{


enum State { Submitted, WaitingToSubmit };

template <class T>
class CommandListInterface
{
public:
	CommandListInterface()
		: _state(Submitted)
		, _fence_value(0)
	{}

	void signal_submitted(ID3D12CommandQueue *command_queue) {
		_fence_value++;
		command_queue->Signal(_fence.Get(), _fence_value);
		_state.store(Submitted, std::memory_order_release);
	}

	ComPtr<T> _command_list;
	ComPtr<ID3D12CommandAllocator> _command_allocator;
	ComPtr<ID3D12Fence> _fence;
	std::atomic<State> _state;
	uint64 _fence_value;
};

typedef std::vector<CommandListInterface<ID3D12GraphicsCommandList>* > GraphicsCommandLists;
typedef std::queue<CommandListInterface<ID3D12GraphicsCommandList>* > CommandListFreeList;

class CommandListFactory
{
public:
	enum CommandListType { Graphics, Compute };
	CommandListFactory(ID3D12Device *device);
	~CommandListFactory();

	static void thread_initialize();
	
	template <class T>
	inline CommandListInterface<T> *aquire_command_list();

	template <class T>
	void release_command_list(ID3D12CommandQueue *command_queue, CommandListInterface<T> *command_list);

private:
	ID3D12Device *_device;
	
	static __THREAD_LOCAL GraphicsCommandLists *_graphics_command_lists;
	static __THREAD_LOCAL CommandListFreeList *_graphics_command_lists_free;
};

template <>
inline CommandListInterface<ID3D12GraphicsCommandList> *CommandListFactory::aquire_command_list()
{
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
	_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_list_storage->_command_allocator));
	_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, command_list_storage->_command_allocator.Get(), nullptr, IID_PPV_ARGS(&command_list_storage->_command_list));
	_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&command_list_storage->_fence));

	_graphics_command_lists->push_back(command_list_storage);
	return command_list_storage;
}

template <>
inline void CommandListFactory::release_command_list(ID3D12CommandQueue *command_queue, CommandListInterface<ID3D12GraphicsCommandList> *command_list_interface)
{
	command_list_interface->_state.store(WaitingToSubmit, std::memory_order_release);
	_graphics_command_lists_free->push(command_list_interface);
}

}