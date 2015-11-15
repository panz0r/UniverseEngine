#pragma once

#include <d3d12.h>
#include <renderer_d3d12/com/com_ptr.h>
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

	// submit a single command list
	void submit(ID3D12CommandQueue *command_queue) {
		_command_list->Close();
		ID3D12CommandList * lists[] = { _command_list.Get() };
		command_queue->ExecuteCommandLists(1, lists);
		signal_submitted(command_queue);
	}


	T* command_list() { return _command_list.Get(); }

	ComPtr<T> _command_list;
	ComPtr<ID3D12CommandAllocator> _command_allocator;
	ComPtr<ID3D12Fence> _fence;
	std::atomic<State> _state;
	uint64_t _fence_value;
};

typedef CommandListInterface<ID3D12GraphicsCommandList> GraphicsCommandList;

}