#pragma once

#include <d3d12.h>
#include "d3dx12.h"
#include <core/assert/assert.h>
#include <renderer_d3d12/com/com_ptr.h>
#include <vector>


namespace ue
{


struct DescriptorHeapHandle
{
	enum { INDEX_BIT = 20, COUNTER_BIT = 12 };
	DescriptorHeapHandle() : _index(0), _counter(0) {}
	DescriptorHeapHandle(unsigned index, unsigned counter)
		: _index(index)
		, _counter(counter)
	{}

	unsigned _index : INDEX_BIT;
	unsigned _counter : COUNTER_BIT;
};


class DescriptorHeap
{
public:

	DescriptorHeap(ID3D12Device &device, D3D12_DESCRIPTOR_HEAP_TYPE type, unsigned descriptor_count_per_heap, bool shader_visible = false)
		: _device(device)
		, _descriptor_heap_type(type)
		, _descriptor_handle_size(device.GetDescriptorHandleIncrementSize(type))
		, _descriptor_count_per_heap(descriptor_count_per_heap)
		, _descriptor_heap_shader_visible(shader_visible)
		, _next_free_index(0)
	{
	}


	DescriptorHeapHandle aquire_handle()
	{
		unsigned index = _next_free_index;

		if (index >= _handles.size()) {
			_handles.resize(_handles.size() + 10);
			for (unsigned i = index; i < _handles.size(); ++i) {
				_handles[i].next_free = i + 1;
			}
		}

		HandleEntry &entry = _handles[index];
		UASSERT(entry.next_free != HandleEntry::IN_USE, "handle in use");
		_next_free_index = entry.next_free;

		// find free heap slot
		if (entry.heap_index == HandleEntry::UNINITIALIZED) {
			unsigned heap_index = 0;
			Heap &heap = find_or_create_heap(heap_index);
			entry.heap_index = heap_index;
			entry.heap_offset = heap._use_count++;
		}

		entry.counter = (entry.counter + 1) % (1 << DescriptorHeapHandle::COUNTER_BIT);
		entry.next_free = HandleEntry::IN_USE;

		DescriptorHeapHandle handle(index, entry.counter);
		return handle;
	}

	void release_handle(DescriptorHeapHandle handle)
	{
		HandleEntry &entry = _handles[handle._index];
		entry.next_free = _next_free_index;
		_next_free_index = handle._index;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE cpu_descriptor_handle(DescriptorHeapHandle handle) const
	{
		const HandleEntry &entry = _handles[handle._index];
		UASSERT(entry.counter == handle._counter, "invalid handle");
		UASSERT(entry.next_free == HandleEntry::IN_USE, "handle was released");

		const Heap &heap = get_heap(entry.heap_index);
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_descriptor_handle(heap.cpu(), entry.heap_offset, _descriptor_handle_size);
		return cpu_descriptor_handle;
	}

	D3D12_GPU_DESCRIPTOR_HANDLE gpu_descriptor_handle(DescriptorHeapHandle handle) const
	{
		const HandleEntry &entry = _handles[handle._index];
		UASSERT(entry.counter == handle._counter, "invalid handle");
		UASSERT(entry.next_free == HandleEntry::IN_USE, "handle was released");
		const Heap &heap = get_heap(entry.heap_index);

		CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_descriptor_handle(heap.gpu(), entry.heap_offset, _descriptor_handle_size);
		return gpu_descriptor_handle;
	}

private:

	struct HandleEntry
	{
		enum { UNINITIALIZED = 0xffffffff, IN_USE = 0xfffff };
		HandleEntry() : next_free(0), counter(0), heap_index(UNINITIALIZED), heap_offset(UNINITIALIZED) {}
		unsigned next_free : DescriptorHeapHandle::INDEX_BIT;
		unsigned counter : DescriptorHeapHandle::COUNTER_BIT;
		unsigned heap_index;
		unsigned heap_offset;
	};

	struct Heap
	{
		inline D3D12_CPU_DESCRIPTOR_HANDLE cpu() const { return _heap->GetCPUDescriptorHandleForHeapStart(); }
		inline D3D12_GPU_DESCRIPTOR_HANDLE gpu() const { return _heap->GetGPUDescriptorHandleForHeapStart(); }
		ComPtr<ID3D12DescriptorHeap> _heap;
		unsigned _use_count;
	};

	inline const Heap &get_heap(unsigned heap_index) const
	{
		return _heaps[heap_index];
	}

	Heap &find_or_create_heap(unsigned &heap_index)
	{
		heap_index = 0;
		for (Heap &heap : _heaps)
		{
			if (heap._use_count < _descriptor_count_per_heap)
				return heap;
			heap_index++;
		}
		_heaps.push_back(new_heap());
		return _heaps.back();
	}

	Heap new_heap()
	{
		Heap heap = {};
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.Type = _descriptor_heap_type;
		desc.NodeMask = 0;
		desc.NumDescriptors = _descriptor_count_per_heap;
		desc.Flags = _descriptor_heap_shader_visible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		_device.CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap._heap));
		return heap;
	}

	ID3D12Device &_device;
	D3D12_DESCRIPTOR_HEAP_TYPE _descriptor_heap_type;
	unsigned _descriptor_handle_size;
	unsigned _descriptor_count_per_heap;
	bool _descriptor_heap_shader_visible;
	unsigned _next_free_index;
	std::vector<Heap> _heaps;
	std::vector<HandleEntry> _handles;
};


}