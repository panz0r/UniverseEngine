#include <renderer_d3d12/stdinclude.h>
#include <renderer_d3d12/d3dx12.h>

#include "d3d12_buffer_writer.h"
#include "d3d12_buffer.h"

#include <renderer_d3d12/memory/d3d12_memory.h>
#include <renderer_d3d12/renderer/d3d12_command_list_factory.h>

namespace ue
{

bool D3D12ResourceWriter::write_buffer(D3D12Buffer* buffer, void* data, uint32_t size, uint32_t alignment, uint32_t type)
{

	D3D12Resource* dest_resource = &buffer->resource;
	uint64_t dest_offset = 0;

	switch (buffer->allocation.allocator_type) {
	case ALLOCATOR_DEFAULT_SUBRESOURCE:
	{
		auto access = d3d12_memory::subresource_access(buffer->allocation);
		dest_offset = access.resource_offset;
	}
	break;
	case ALLOCATOR_UPLOAD_HEAP:
	{
		// immediate update
		auto access = d3d12_memory::subresource_access(buffer->allocation);
		void* dest_data = access.mapped_data->map(access.resource_offset);
		memcpy(dest_data, data, size);
		access.mapped_data->unmap();
		return true;
	}
	break;
	}

	D3D12RingBufferAllocator* allocator = d3d12_memory::upload_ringbuffer_allocator();
	D3D12RingBufferAllocator::D3D12RingBuffer* ring_buffer = allocator->buffer();
	uint32_t upload_offset = allocator->allocate(size, alignment);
	if (upload_offset == D3D12RingBufferAllocator::INVALID_ALLOCATION)
		return false;

	memcpy(ring_buffer->mapped_data + upload_offset, data, size);

	ResourceItem item;
	item.dest_resource = dest_resource;
	item.dest_offset = dest_offset;
	item.type = type;
	item.src_resource = ring_buffer->resource.Get();
	item.src_offset = upload_offset;
	item.size = size;

	_items.push_back(item);
	
	return true;

}

void D3D12ResourceWriter::dispatch(ID3D12GraphicsCommandList* command_list)
{
	D3D12Transition transition;

	for (ResourceItem& item : _items)
	{
		transition.barrier(item.dest_resource->resource.Get(), item.dest_resource->resource_state, D3D12_RESOURCE_STATE_COPY_DEST, true);
	}

	transition.apply(command_list);

	for (ResourceItem& item : _items)
	{
		copy_subresource(command_list, item.dest_resource->resource.Get(), item.dest_offset, item.src_resource, item.src_offset, item.size);
	}

	transition.revert(command_list);
}

void D3D12ResourceWriter::copy_subresource(ID3D12GraphicsCommandList* command_list, ID3D12Resource* dest_resource, uint64_t dest_offset, ID3D12Resource* src_resource, uint64_t src_offset, uint32_t size)
{
	command_list->CopyBufferRegion(dest_resource, dest_offset, src_resource, src_offset, size);
}



//////////////////////////////////////


D3D12DynamicBufferWriter::D3D12DynamicBufferWriter()
	: _buffer(nullptr)
	, _mapped_data(nullptr)
{
}

D3D12DynamicBufferWriter::D3D12DynamicBufferWriter(D3D12Buffer* buffer)
	: _buffer(buffer)
	, _mapped_data(nullptr)
{
	UENSURE(buffer->dynamic);
	map_data();
}

D3D12DynamicBufferWriter::~D3D12DynamicBufferWriter()
{
	unmap_data();
}

void D3D12DynamicBufferWriter::write(void* data, size_t size)
{
	memcpy(_mapped_data, data, size);
}

void D3D12DynamicBufferWriter::map_data()
{
	auto access = d3d12_memory::subresource_access(_buffer->allocation);
	_mapped_data = reinterpret_cast<void*>(reinterpret_cast<size_t>(access.mapped_data->map()) + access.resource_offset);
}

void D3D12DynamicBufferWriter::unmap_data()
{
	auto access = d3d12_memory::subresource_access(_buffer->allocation);
	access.mapped_data->unmap();
}

////////////////////////////////////////////////////////


D3D12BufferWriter::D3D12BufferWriter()
	: _buffer(nullptr)
{
}

D3D12BufferWriter::D3D12BufferWriter(D3D12Buffer* buffer)
	: _buffer(buffer)
{
	UENSURE(_buffer->dynamic == false);
}

D3D12BufferWriter::~D3D12BufferWriter()
{
}

void D3D12BufferWriter::write(void* data, size_t size, D3D12GraphicsCommandList& command_list)
{

	ID3D12Resource* dest_resource = _buffer->resource.resource.Get();
	uint64 dest_offset = 0;

	if (_buffer->allocation.allocator_type == ALLOCATOR_DEFAULT_SUBRESOURCE) {
		auto dest_access = d3d12_memory::subresource_access(_buffer->allocation);
		dest_resource = dest_access.resource.resource.Get();
		dest_offset = dest_access.resource_offset;
	}

	D3D12Allocation intermediate_allocation = d3d12_memory::upload_subresource_allocator()->allocate(size, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
	D3D12SubResourceAccess source_access = d3d12_memory::subresource_access(intermediate_allocation);

	void* ptr = source_access.mapped_data->map(source_access.resource_offset);

	D3D12_SUBRESOURCE_DATA subresource_data;
	subresource_data.pData = data;
	subresource_data.RowPitch = size;
	subresource_data.SlicePitch = 0;
	
	auto cmd_list = command_list();
	auto source_resource = source_access.resource.resource.Get();
	subresource_copy(cmd_list, dest_resource, dest_offset, 
		source_resource, ptr, source_access.resource_offset, &subresource_data);

	source_access.mapped_data->unmap();
}

void D3D12BufferWriter::subresource_copy(ID3D12GraphicsCommandList* command_list,
	ID3D12Resource* dest_resource, uint64_t dest_offset,
	ID3D12Resource* intermediate_resource, void* intermediate_data, uint64_t src_offset,
	const D3D12_SUBRESOURCE_DATA* subresource_data)
{

	uint64_t total_bytes;
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT* layouts = reinterpret_cast<D3D12_PLACED_SUBRESOURCE_FOOTPRINT*>(_mem);
	uint64_t* row_sizes_in_bytes = reinterpret_cast<uint64_t*>(layouts + 1);
	unsigned* num_rows = reinterpret_cast<unsigned*>(row_sizes_in_bytes + 1);

	D3D12_RESOURCE_DESC desc = dest_resource->GetDesc();
	ID3D12Device* device;
	dest_resource->GetDevice(IID_PPV_ARGS(&device));
	device->GetCopyableFootprints(&desc, 0, 1, dest_offset, layouts, num_rows, row_sizes_in_bytes, &total_bytes);
	device->Release();

	memcpy(intermediate_data, subresource_data->pData, subresource_data->RowPitch);
	command_list->CopyBufferRegion(dest_resource, dest_offset, intermediate_resource, src_offset, subresource_data->RowPitch);

}

}