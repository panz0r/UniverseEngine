#pragma once

//***********************************************
// Based on ijcringbuffer by incrediblejr
// https://github.com/incrediblejr/ijcringbuffer

#include <renderer_d3d12/stdinclude.h>
#include <core/assert/assert.h>


namespace ue
{

class D3D12RenderDevice;

class D3D12RingBufferAllocator
{
public:
	enum { INVALID_ALLOCATION = (uint32_t)-1 };
	struct D3D12RingBuffer
	{
		ComPtr<ID3D12Resource> resource;
		uint8_t *mapped_data;
	};

public:
	D3D12RingBufferAllocator(D3D12RenderDevice &device, uint32_t size);
	~D3D12RingBufferAllocator();
	
	void set_fence();
	void wait_for_fence();

	uint32_t allocate(uint32_t size, uint32_t align = 4U);
	uint32_t consume_to_signaled_fence();
	D3D12RingBuffer* buffer() { return &_buffer; }

private:
	uint32_t consume_to_marker(uint32_t marker);

	uint32_t difference(uint32_t a, uint32_t b) {
		uint32_t ab = a - b;
		uint32_t ba = b - a;
		return ab > ba ? ba : ab;
	}
	
	bool is_split(uint32_t write_marker) {
		return difference(_read, write_marker) > _size;
	}

	uint32_t consumable_size(uint32_t write_marker, bool contigious) {
		uint32_t cs;
		if (is_split(write_marker)) {
			if (_read == _wrap) {
				unsigned masked_write = write_marker&_mask;
				cs = masked_write ? masked_write : _size;
			}
			else {
				UENSURE(_size > difference(_wrap, _read));
				cs = ((_wrap - _read)&_mask) + (contigious ? 0 : (write_marker & _mask));
			}
		}
		else {
			cs = write_marker - _read;
		}
		UENSURE(_size >= cs);
		return cs;
	}

	__forceinline bool is_empty() { return (_read == _write); }
	__forceinline bool is_full() { return consumable_size(_write, false) == _size; }

	uint32_t produce(uint32_t size);
	void consume(uint32_t write_marker, uint32_t size);

private:
	D3D12RenderDevice &_device;

	// Internal
	uint32_t _size, _write, _read, _wrap, _mask;

	// Resource
	D3D12RingBuffer _buffer;
	
	// Fence
	ComPtr<ID3D12Fence> _fence;
	HANDLE _fence_event;

	struct FenceValue
	{
		union {
			uint64_t fence_value;
			uint32_t write;
			uint32_t index;
		};
	};
	uint32_t _fence_index;
	uint32_t _signaled_fence_index;
};


}