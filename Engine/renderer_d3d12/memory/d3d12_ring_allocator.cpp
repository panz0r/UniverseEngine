#include "d3d12_ring_allocator.h"
#include <renderer_d3d12/renderer/d3d12_render_device.h>
#include <Windows.h>

namespace ue
{

D3D12RingBufferAllocator::D3D12RingBufferAllocator(D3D12RenderDevice &device, uint32_t size)
	: _device(device)
	, _size(size)
	, _read(0)
	, _write(0)
	, _wrap(0)
	, _mask((uint32_t)(_size-1))
	, _fence_index(1)
	, _signaled_fence_index(0)
	
{
	HRESULT hr = _device.device()->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(size),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_buffer.resource));
	UENSURE(SUCCEEDED(hr));
	hr = _buffer.resource->Map(0, nullptr, (void**)&_buffer.mapped_data);
	UENSURE(SUCCEEDED(hr));
	hr = _device.device()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	UENSURE(SUCCEEDED(hr));
	_fence_event = CreateEventEx(nullptr, 0, 0, EVENT_ALL_ACCESS);
}

D3D12RingBufferAllocator::~D3D12RingBufferAllocator()
{
	_buffer.resource->Unmap(0, nullptr);
	CloseHandle(_fence_event);
}

void D3D12RingBufferAllocator::set_fence()
{
	FenceValue value;
	value.write = _write;
	value.index = _fence_index++;
	_device.command_queue()->Signal(_fence.Get(), value.fence_value);
}

void D3D12RingBufferAllocator::wait_for_fence()
{
	// Block until the entire ringbuffer is free
	// Make sure all command lists that have allocated stuff on the ring buffer
	// have been executed before calling this!
	FenceValue value;
	value.write = _write;
	value.index = _fence_index++;
	_device.command_queue()->Signal(_fence.Get(), value.fence_value);

	if(_fence->GetCompletedValue() != value.fence_value) {
		_fence->SetEventOnCompletion(value.fence_value, _fence_event);
		WaitForSingleObject(_fence_event, INFINITE);
		consume_to_signaled_fence();
	}

}

uint32_t D3D12RingBufferAllocator::consume_to_signaled_fence()
{
	FenceValue value;
	value.fence_value = _fence->GetCompletedValue();

	if (value.index <= _signaled_fence_index)
		return 0;

	_signaled_fence_index = value.index;
	return consume_to_marker(value.write);
}

uint32_t D3D12RingBufferAllocator::consume_to_marker(uint32_t marker)
{
	uint32_t largest_contigious = 0;
	uint32_t total_amount_to_consume = consumable_size(marker, false);
	uint32_t remaining = total_amount_to_consume;
	while (remaining) {
		uint32_t contigious_consumable_size = consumable_size(marker, true);
		if(contigious_consumable_size > largest_contigious)
			largest_contigious = contigious_consumable_size;
		uint32_t consume_amount = (contigious_consumable_size < remaining) ? contigious_consumable_size : remaining;
		consume(marker, consume_amount);
		remaining -= consume_amount;
	}

	return largest_contigious;	// return largest contigious part consumed
}

uint32_t D3D12RingBufferAllocator::allocate(uint32_t size, uint32_t align)
{
	uint32_t align_diff = difference(_write, ALIGN(_write, align));
	uint32_t required_size = size + align_diff;
	uint32_t offset = produce(required_size);
	
	if(offset == INVALID_ALLOCATION)
		return offset;

	uint32_t aligned_offset = ALIGN(offset, align);
	return aligned_offset;
}


uint32_t D3D12RingBufferAllocator::produce(uint32_t size)
{
	uint32_t masked_write = _write & _mask;

	if (is_split(_write)) {
		uint32_t available_size;
		if (_wrap == _read) {
			available_size = masked_write ? (_size - masked_write) : 0;
		}
		else {
			UENSURE((_read & _mask) >= (_write & _mask));
			available_size = (_read - _write) & _mask;
		}

		if (available_size >= size) {
			uint32_t ret = _write & _mask;
			_write += size;
			return ret;
		}
		goto failed;
	}

	bool empty = is_empty();

	if(masked_write && empty && _size >= size)
		goto write_front;

	if (!masked_write && !empty) {
		goto check_front;
	}

	if ((_size - masked_write) >= size) {
		uint32_t ret = _write & _mask;
		_write += size;
		return ret;
	}

check_front:
	if ((_read & _mask) >= size) {
write_front:
		_wrap = _write;
		uint32_t ret = _write & _mask;
		_write += _size + (_size - masked_write) + size;
		return ret;
	}
failed:
	uint32_t amount_consumed = consume_to_signaled_fence();
	if(amount_consumed >= size)
		return produce(size);	// try again

	return INVALID_ALLOCATION;
}

void D3D12RingBufferAllocator::consume(uint32_t write_marker, uint32_t size) {
	UENSURE(consumable_size(write_marker, false) >= size);
	if(_read == _wrap && is_split(write_marker))
		_read += _size + (_size - (_read & _mask)) + size;
	else
		_read += size;
}

}