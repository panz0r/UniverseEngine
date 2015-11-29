#pragma once

#include <d3d12.h>
#include "d3dx12.h"
#include <vector>
#include "online_descriptor_heap.h"
#include <renderer_d3d12/resource/common.h>

namespace ue
{

enum
{
	MAX_VERTEX_BUFFERS = 8,
};

struct RenderAtomDesc
{
	unsigned topology;
	RenderHandle pso;
	RenderHandle index_buffer;
	unsigned vertex_buffer_count;
	RenderHandle vertex_buffers[MAX_VERTEX_BUFFERS];
};

struct InstancedRenderAtomDesc : public RenderAtomDesc
{
	unsigned index_count_per_instance;
	unsigned instance_count;
	unsigned start_index_location;
	unsigned base_vertex_location;
	unsigned start_instance_location;
};


struct RootParameterDesc
{
	unsigned index;
	unsigned type;
	union
	{
		struct _RootConstant
		{
			unsigned num_values;
			unsigned dest_offset;
		} root_constant;

		struct _DescriptorTableEntry
		{
			unsigned table_index;
		} descriptor_table_entry;
	};
};

struct RootParameterValue
{
	union 
	{
		unsigned render_resource;
		void * constant_data;
	};
};



struct RootParameterConstants
{
	unsigned num_values;
	unsigned dest_offset;
	unsigned * values;	// can be external pointer or internal pointer
};

struct RootParameterDescriptorTable
{
	unsigned num_source_descriptors;
	unsigned pad; // to maintain correct pointer alignment within render atom on x64
};

struct RootParameter
{
	enum { ROOT_CONSTANT, ROOT_MULTIPLE_CONSTANTS, ROOT_CBV, ROOT_SRV, ROOT_UAV, ROOT_DESCRIPTOR_TABLE, ROOT_DESCRIPTOR_TABLE_SAMPLER };
	unsigned size; // full count of root parameter + data
	unsigned index;
	unsigned type;
	unsigned hash;	// hash of this bind point, to determine if the root parameter needs to be rebound or not
};


struct RenderAtom : public RenderResource
{
	size_t size;	// count of entire render atom
};

_declspec(align(8)) struct InstancedRenderAtom : public RenderAtom
{
	static const ResourceType Type = RenderResource::InstancedRenderAtom;
	// required render data
	unsigned index_count_per_instance;
	unsigned instance_count;
	unsigned start_index_location;
	unsigned base_vertex_location;
	unsigned start_instance_location;
	unsigned topology;

	// resource count
	unsigned vertex_buffer_count;
	unsigned root_parameter_count;
};


struct DescriptorHeaps
{
	OnlineDescriptorHeap *cbv_srv_uav_heap;
	OnlineDescriptorHeap *sampler_heap;
};

struct StateCache
{
	ID3D12PipelineState *pipeline_state;
	unsigned topology;
	void *vertx_buffers;
	void *index_buffer;
	unsigned srv_hash; // deprecated
	unsigned uav_hash; // deprecated
	unsigned cbv_hash; // deprecated
	unsigned sampler_hash; // deprecated
	unsigned root_paramater_hash[32];	// should be inplace allocated based on number of root parameters
};

// returns true if hash was updated, false if both hashes are the same, updates the reference hash
template <typename T>
inline bool state_changed(T current, T &reference)
{
	bool changed = (current != reference);
	reference = current;
	return changed;
}

template <typename T>
T ptr_offset(void *ptr, size_t size) {
	return reinterpret_cast<T>(reinterpret_cast<size_t>(ptr) + size);
}

void draw_instanced_render_atom(InstancedRenderAtom *atom,
	ID3D12GraphicsCommandList *command_list,
	ID3D12Device *device,
	DescriptorHeaps *descriptor_heaps,
	StateCache *state_cache);

} // ue