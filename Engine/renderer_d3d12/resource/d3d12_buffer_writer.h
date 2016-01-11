#pragma once
#include <stdint.h>
#include <vector>

namespace ue
{

struct D3D12Resource;
struct D3D12Buffer;
struct D3D12GraphicsCommandList;

class D3D12Transition
{
public:
	D3D12Transition() {}
	~D3D12Transition() {}

	void barrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after, bool reverseable)
	{
		if(before == after)
			return;

		// check for duplicates
		for (auto& barrier : _barriers) {
			if(barrier.Transition.pResource == resource)
				return;
		}

		D3D12_RESOURCE_BARRIER barrier = 
		{
			D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
			D3D12_RESOURCE_BARRIER_FLAG_NONE,
			{ resource, 0, before, after }
		};
		_barriers.push_back(barrier);
		
		if (reverseable) {
			barrier.Transition.StateBefore = after;
			barrier.Transition.StateAfter = before;
			_reverse_barriers.push_back(barrier);
		}
	}

	void apply(ID3D12GraphicsCommandList* command_list)
	{
		command_list->ResourceBarrier(_barriers.size(), &_barriers[0]);
	}

	void revert(ID3D12GraphicsCommandList* command_list)
	{
		command_list->ResourceBarrier(_reverse_barriers.size(), &_reverse_barriers[0]);
	}

private:

	std::vector<D3D12_RESOURCE_BARRIER> _barriers;
	std::vector<D3D12_RESOURCE_BARRIER> _reverse_barriers;
};

class D3D12ResourceWriter
{
private:
	struct ResourceItem
	{
		uint32_t type;
		D3D12Resource* dest_resource;
		uint64_t dest_offset;
		ID3D12Resource* src_resource;
		uint32_t src_offset;
		uint32_t size;
	};

public:
	enum { BUFFER, CONSTANT_BUFFER, TEXTURE };
	D3D12ResourceWriter() {}
	~D3D12ResourceWriter() {}

	bool write_buffer(D3D12Buffer* buffer, void* data, uint32_t size, uint32_t alignment, uint32_t type);
	void write_texture() {}

	void dispatch(ID3D12GraphicsCommandList* command_list);

private:

	void copy_subresource(ID3D12GraphicsCommandList* command_list, ID3D12Resource* dest_resource, uint64_t dest_offset, ID3D12Resource* src_resource, uint64_t src_offset, uint32_t size);

	std::vector<ResourceItem> _items;

};

class D3D12DynamicBufferWriter
{
public:
	D3D12DynamicBufferWriter();
	D3D12DynamicBufferWriter(D3D12Buffer* buffer);
	~D3D12DynamicBufferWriter();
	
	void write(void* data, size_t size);

private:
	void map_data();
	void unmap_data();
	D3D12Buffer* _buffer;
	void* _mapped_data;
};


class D3D12BufferWriter
{
public:
	D3D12BufferWriter();
	D3D12BufferWriter(D3D12Buffer* buffer);
	~D3D12BufferWriter();

	void write(void* data, size_t size, D3D12GraphicsCommandList& command_list);
private:

	void subresource_copy(ID3D12GraphicsCommandList* command_list,
		ID3D12Resource* dest_resource, uint64_t dest_offset,
		ID3D12Resource* intermediate_resource, void* intermediate_data, uint64_t src_offset,
		const D3D12_SUBRESOURCE_DATA* subresource_data);

private:
	D3D12Buffer* _buffer;
	char _mem[sizeof(D3D12_PLACED_SUBRESOURCE_FOOTPRINT) + sizeof(unsigned) + sizeof(uint64_t)];
};

}