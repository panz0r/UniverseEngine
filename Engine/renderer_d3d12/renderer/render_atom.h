#pragma once

#include <d3d12.h>
#include "d3dx12.h"
#include <vector>
#include "descriptor_heap_factory.h"
#include "resource_manager.h"

namespace ue
{

enum 
{ 
	MAX_VERTEX_BUFFERS = 8, 
	MAX_CBVS = 8,
	MAX_UAVS = 16,
	MAX_SRVS = 16,
	MAX_SAMPLERS = 16,
};

struct BaseRenderAtomDesc
{
	unsigned topology;
	RenderHandle pso;
	RenderHandle index_buffer;
	unsigned vertex_buffer_count;
	RenderHandle vertex_buffers[MAX_VERTEX_BUFFERS];
	unsigned uav_count;
	RenderHandle uavs[MAX_UAVS];
	unsigned srv_count;
	RenderHandle srvs[MAX_SRVS];
	unsigned cbv_count;
	RenderHandle cbvs[MAX_CBVS];
	unsigned sampler_count;
	RenderHandle samplers[MAX_SAMPLERS];

};

struct InstancedRenderAtomDesc : public BaseRenderAtomDesc
{
	unsigned index_count_per_instance;
	unsigned instance_count;
	unsigned start_index_location;
	unsigned base_vertex_location;
	unsigned start_instance_location;
};

struct RenderAtomBase
{
	size_t size;	// size of entire render atom
};

struct InstancedRenderAtom : public RenderAtomBase
{
	// required render data
	unsigned index_count_per_instance;
	unsigned instance_count;
	unsigned start_index_location;
	unsigned base_vertex_location;
	unsigned start_instance_location;
	unsigned topology;


	// resource count
	unsigned vertex_buffer_count;
	unsigned srv_count;
	unsigned uav_count;
	unsigned cbv_count;
	unsigned sampler_count;
	
	// frame stride for dynamic resources
	unsigned vertex_buffer_stride;
	unsigned srv_stride;
	unsigned uav_stride;
	unsigned cbv_stride;

	// hashes of handles to avoid resetting the same tables
	// saving some memory on the runtime heap
	unsigned srv_hash;
	unsigned uav_hash;
	unsigned cbv_hash;
	unsigned sampler_hash;

	// resources, following the RenderAtom struct

	// pipeline_state_object
	// index_buffer;
	// vertex_buffers;
	// srvs
	// uavs
	// cbvs
	// samplers
};

struct DescriptorHeaps
{
	RuntimeDescriptorHeap *cbv_srv_uav_heap;
	RuntimeDescriptorHeap *sampler_heap;
};

struct StateCache
{
	ID3D12PipelineState *pipeline_state;
	unsigned srv_hash;
	unsigned uav_hash;
	unsigned cbv_hash;
	unsigned sampler_hash;
};

// returns true if hash was updated, false if both hashes are the same
inline bool update_hash(unsigned hash, unsigned &cached_hash) {
	bool ret = (hash != cached_hash);
	cached_hash = hash;
	return ret;
};

inline void draw_instanced_render_atom(InstancedRenderAtom *atom,
	ID3D12GraphicsCommandList *command_list,
	ID3D12Device *device,
	DescriptorHeaps *descriptor_heaps,
	StateCache *state_cache)
{
	static const unsigned cbv_srv_uav_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	static const unsigned sampler_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

	char* resource_ptr = reinterpret_cast<char*>(atom) + sizeof(InstancedRenderAtom);

	// Map resources
	auto root_signature = reinterpret_cast<ID3D12RootSignature*>(resource_ptr);
	resource_ptr += sizeof(ID3D12RootSignature*);
	auto pipeline_state = reinterpret_cast<ID3D12PipelineState*>(resource_ptr);
	resource_ptr += sizeof(ID3D12PipelineState*);
	auto index_buffer = reinterpret_cast<D3D12_INDEX_BUFFER_VIEW*>(resource_ptr);
	resource_ptr += sizeof(D3D12_INDEX_BUFFER_VIEW);
	auto vertex_buffers = reinterpret_cast<D3D12_VERTEX_BUFFER_VIEW*>(&resource_ptr);
	resource_ptr += sizeof(D3D12_VERTEX_BUFFER_VIEW) * atom->vertex_buffer_count;
	auto srvs = reinterpret_cast<D3D12_CPU_DESCRIPTOR_HANDLE*>(resource_ptr);
	resource_ptr += sizeof(D3D12_CPU_DESCRIPTOR_HANDLE) * atom->srv_count;
	auto uavs = reinterpret_cast<D3D12_CPU_DESCRIPTOR_HANDLE*>(resource_ptr);
	resource_ptr += sizeof(D3D12_CPU_DESCRIPTOR_HANDLE) * atom->uav_count;
	auto cbvs = reinterpret_cast<D3D12_CPU_DESCRIPTOR_HANDLE*>(resource_ptr);
	resource_ptr += sizeof(D3D12_CPU_DESCRIPTOR_HANDLE) * atom->cbv_count;
	auto samplers = reinterpret_cast<D3D12_CPU_DESCRIPTOR_HANDLE*>(resource_ptr);

	// Set pipeline state
	if (pipeline_state != state_cache->pipeline_state) {
		command_list->SetPipelineState(pipeline_state);
		state_cache->pipeline_state = pipeline_state;
	}

	// find dirty descriptor counts
	unsigned srv_count = update_hash(atom->srv_hash, state_cache->srv_hash) ? atom->srv_count : 0;
	unsigned uav_count = update_hash(atom->uav_hash, state_cache->uav_hash) ? atom->uav_count : 0;
	unsigned cbv_count = update_hash(atom->cbv_hash, state_cache->cbv_hash) ? atom->cbv_count : 0;
	unsigned dirty_srv_uav_cbv_count = srv_count + uav_count + cbv_count;

	descriptor_heaps->cbv_srv_uav_heap->require(dirty_srv_uav_cbv_count);
	auto &cbv_srv_uav_gpu_handle = descriptor_heaps->cbv_srv_uav_heap->_gpu_handle;
	auto &cbv_srv_uav_cpu_handle = descriptor_heaps->cbv_srv_uav_heap->_cpu_handle;

	// build cbv_srv_uav descriptor copy info
	// TODO, make scratch pad for entire task
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> src_handels;
	std::vector<unsigned> src_sizes;

	for (unsigned i = 0; i < srv_count; ++i) {
		src_handels.push_back(srvs[i]);
		src_sizes.push_back(1);
	}

	for (unsigned i = 0; i < uav_count; ++i) {
		src_handels.push_back(uavs[i]);
		src_sizes.push_back(1);
	}

	for (unsigned i = 0; i < cbv_count; ++i) {
		src_handels.push_back(cbvs[i]);
		src_sizes.push_back(1);
	}


	// Copy dirty descriptors to a linear table
	if (dirty_srv_uav_cbv_count > 0) {
		device->CopyDescriptors(1, &cbv_srv_uav_cpu_handle, &dirty_srv_uav_cbv_count,
			dirty_srv_uav_cbv_count, &src_handels[0], &src_sizes[0], D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	// bind dirty cbv_srv_uav descriptors
	if (srv_count > 0) {
		command_list->SetGraphicsRootDescriptorTable(0, cbv_srv_uav_gpu_handle);
		cbv_srv_uav_gpu_handle.Offset(srv_count, cbv_srv_uav_descriptor_size);
	}
	if (uav_count > 0) {
		command_list->SetGraphicsRootDescriptorTable(1, cbv_srv_uav_gpu_handle);
		cbv_srv_uav_gpu_handle.Offset(uav_count, cbv_srv_uav_descriptor_size);
	}
	if (cbv_count > 0) {
		command_list->SetGraphicsRootDescriptorTable(2, cbv_srv_uav_gpu_handle);
		cbv_srv_uav_gpu_handle.Offset(cbv_count, cbv_srv_uav_descriptor_size);
	}

	cbv_srv_uav_cpu_handle.Offset(dirty_srv_uav_cbv_count, cbv_srv_uav_descriptor_size);

	// find dirty sampler count
	unsigned dirty_sampler_count = update_hash(atom->sampler_hash, state_cache->sampler_hash) ? atom->sampler_count : 0;

	// build sampler copy info
	src_handels.clear();
	src_sizes.clear();
	for (unsigned i = 0; i < dirty_sampler_count; ++i) {
		src_handels.push_back(samplers[i]);
		src_sizes.push_back(1);
	}

	descriptor_heaps->sampler_heap->require(dirty_sampler_count);
	auto &sampler_gpu_handle = descriptor_heaps->sampler_heap->_gpu_handle;
	auto &sampler_cpu_handle = descriptor_heaps->sampler_heap->_cpu_handle;


	// copy and bind dirty samplers
	if (dirty_sampler_count > 0) {
		device->CopyDescriptors(1, &sampler_cpu_handle, &dirty_sampler_count,
			dirty_sampler_count, &src_handels[0], &src_sizes[0], D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
		command_list->SetGraphicsRootDescriptorTable(3, sampler_gpu_handle);
		sampler_gpu_handle.Offset(dirty_sampler_count, sampler_descriptor_size);
		sampler_cpu_handle.Offset(dirty_sampler_count, sampler_descriptor_size);
	}

	// setup render resources
	command_list->IASetPrimitiveTopology(static_cast<D3D12_PRIMITIVE_TOPOLOGY>(atom->topology));
	command_list->IASetIndexBuffer(index_buffer);
	command_list->IASetVertexBuffers(0, atom->vertex_buffer_count, vertex_buffers);

	// draw
	command_list->DrawIndexedInstanced(atom->index_count_per_instance,
		atom->instance_count,
		atom->start_index_location,
		atom->base_vertex_location,
		atom->start_instance_location);
}


class RenderAtomFactory
{
public:
	RenderAtomFactory(ID3D12Device *device);
	~RenderAtomFactory();

	size_t compile_placed_render_atom(const InstancedRenderAtomDesc &desc, void *buffer, size_t size);

private:

	ID3D12Device *_device;
	ResourceManager *_resource_manager;
};

} // ue