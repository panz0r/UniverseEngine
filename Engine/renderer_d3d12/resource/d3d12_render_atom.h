#pragma once

namespace ue
{


struct D3D12RootParameterDesc
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

struct D3D12RootParameterValue
{
	union
	{
		RenderHandle render_resource;
		void * constant_data;
	};
};


struct D3D12RootParameterConstants
{
	unsigned num_values;
	unsigned dest_offset;
//	unsigned * values;	// can be external pointer or internal pointer
};

struct D3D12RootParameterDescriptorTable
{
	unsigned num_source_descriptors;
	unsigned pad; // to maintain correct pointer alignment within render atom on x64
};


struct D3D12RootParameter
{
	enum { ROOT_CONSTANT, ROOT_MULTIPLE_CONSTANTS, ROOT_CBV, ROOT_SRV, ROOT_UAV, ROOT_DESCRIPTOR_TABLE, ROOT_DESCRIPTOR_TABLE_SAMPLER };
	unsigned size; // full count of root parameter + mapped_data
	unsigned index;
	unsigned type;
	unsigned hash;	// hash of this bind point, to determine if the root parameter needs to be rebound or not
};



struct D3D12InstancedRenderAtom
{
	unsigned index_count_per_instance;
	unsigned instance_count;
	unsigned start_index_location;
	unsigned base_vertex_location;
	unsigned start_instance_location;
	unsigned topology;

	unsigned vertex_buffer_count;
	unsigned root_parameter_count;
};


}