#include "render_atom.h"

namespace ue {


void draw_instanced_render_atom(InstancedRenderAtom *atom,
	ID3D12GraphicsCommandList *command_list,
	ID3D12Device *device,
	DescriptorHeaps *descriptor_heaps,
	StateCache *state_cache)
{
	static const unsigned cbv_srv_uav_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	static const unsigned sampler_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

	char* resource_ptr = reinterpret_cast<char*>(atom) + sizeof(InstancedRenderAtom);

	// Map resources
	auto pipeline_state = *reinterpret_cast<ID3D12PipelineState**>(resource_ptr);
	resource_ptr += sizeof(ID3D12PipelineState*);
	auto index_buffer = reinterpret_cast<D3D12_INDEX_BUFFER_VIEW*>(resource_ptr);
	resource_ptr += sizeof(D3D12_INDEX_BUFFER_VIEW);
	auto vertex_buffers = reinterpret_cast<D3D12_VERTEX_BUFFER_VIEW*>(resource_ptr);
	resource_ptr += sizeof(D3D12_VERTEX_BUFFER_VIEW) * atom->vertex_buffer_count;
	auto root_parameters = reinterpret_cast<RootParameter*>(resource_ptr);

	// Set pipeline state
	if (state_changed(pipeline_state, state_cache->pipeline_state))
		command_list->SetPipelineState(pipeline_state);

	auto &cbv_srv_uav_gpu_handle = descriptor_heaps->cbv_srv_uav_heap->_gpu_handle;
	auto &cbv_srv_uav_cpu_handle = descriptor_heaps->cbv_srv_uav_heap->_cpu_handle;

	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> src_handles;
	std::vector<unsigned> src_sizes;
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> dest_handles;
	std::vector<unsigned> dest_sizes;

	dest_handles.push_back(descriptor_heaps->cbv_srv_uav_heap->_cpu_handle);
	dest_sizes.push_back(0);

	RootParameter *parameter = root_parameters;
	for (unsigned i = 0; i < atom->root_parameter_count; ++i) {
		void *parameter_data = ptr_offset<void*>(parameter, sizeof(RootParameter));

		// ignore root parameters that's already set to the same values
		if (!state_changed(parameter->hash, state_cache->root_paramater_hash[i])) {
			parameter = ptr_offset<RootParameter*>(parameter, parameter->size);
			continue;
		}

		switch (parameter->type)
		{
		case RootParameter::ROOT_CONSTANT:
		{
			auto root_parameter_constants = reinterpret_cast<RootParameterConstants*>(parameter_data);
			command_list->SetGraphicsRoot32BitConstant(parameter->index, *root_parameter_constants->values, root_parameter_constants->dest_offset);
			break;
		}
		case RootParameter::ROOT_MULTIPLE_CONSTANTS:
		{
			auto root_parameter_constants = reinterpret_cast<RootParameterConstants*>(parameter_data);
			command_list->SetGraphicsRoot32BitConstants(parameter->index, root_parameter_constants->num_values, root_parameter_constants->values, root_parameter_constants->dest_offset);
			break;
		}
		case RootParameter::ROOT_CBV:
		{
			auto gpu_virtual_address = *reinterpret_cast<D3D12_GPU_VIRTUAL_ADDRESS*>(parameter_data);
			command_list->SetGraphicsRootConstantBufferView(parameter->index, gpu_virtual_address);
			break;
		}
		case RootParameter::ROOT_SRV:
		{
			auto gpu_virtual_address = *reinterpret_cast<D3D12_GPU_VIRTUAL_ADDRESS*>(parameter_data);
			command_list->SetGraphicsRootShaderResourceView(parameter->index, gpu_virtual_address);
			break;
		}
		case RootParameter::ROOT_UAV:
		{
			auto gpu_virtual_address = *reinterpret_cast<D3D12_GPU_VIRTUAL_ADDRESS*>(parameter_data);
			command_list->SetGraphicsRootUnorderedAccessView(parameter->index, gpu_virtual_address);
			break;
		}
		case RootParameter::ROOT_DESCRIPTOR_TABLE:
		{
			auto source_descriptors = reinterpret_cast<RootParameterDescriptorTable*>(parameter_data);

			if (!descriptor_heaps->cbv_srv_uav_heap->require(source_descriptors->num_source_descriptors)) {
				dest_handles.push_back(descriptor_heaps->cbv_srv_uav_heap->_cpu_handle);
				dest_sizes.push_back(0);
			}

			auto &current_dest_gpu_handle = descriptor_heaps->cbv_srv_uav_heap->_gpu_handle;
			auto &current_dest_cpu_handle = descriptor_heaps->cbv_srv_uav_heap->_cpu_handle;

			// bind to the current position on the runtime heap and setup source descriptors to be copied to the heap
			command_list->SetGraphicsRootDescriptorTable(parameter->index, current_dest_gpu_handle);
			current_dest_gpu_handle.Offset(source_descriptors->num_source_descriptors, cbv_srv_uav_descriptor_size);
			current_dest_cpu_handle.Offset(source_descriptors->num_source_descriptors, cbv_srv_uav_descriptor_size);

			dest_sizes.back() += source_descriptors->num_source_descriptors;

			auto cpu_source_handles = ptr_offset<D3D12_CPU_DESCRIPTOR_HANDLE*>(source_descriptors, sizeof(RootParameterDescriptorTable));
			for (unsigned j = 0; j < source_descriptors->num_source_descriptors; ++j) {
				src_handles.push_back(cpu_source_handles[j]);
				src_sizes.push_back(1);
			}
		}
		break;
		// TODO, Sampler descriptor table
		}

		// next parameter
		parameter = ptr_offset<RootParameter*>(parameter, parameter->size);
	}

	// copy descriptors to runtime heap
	if (src_handles.size() > 0) {
		device->CopyDescriptors(dest_handles.size(), &dest_handles[0], &dest_sizes[0],
			src_handles.size(), &src_handles[0], &src_sizes[0], D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	// setup other render resources
	if (state_changed(atom->topology, state_cache->topology))
		command_list->IASetPrimitiveTopology(static_cast<D3D12_PRIMITIVE_TOPOLOGY>(atom->topology));
	if (state_changed((void*)index_buffer, state_cache->index_buffer))
		command_list->IASetIndexBuffer(index_buffer);
	if (state_changed((void*)vertex_buffers, state_cache->vertx_buffers))
		command_list->IASetVertexBuffers(0, atom->vertex_buffer_count, vertex_buffers);

	// draw
	command_list->DrawIndexedInstanced(atom->index_count_per_instance,
		atom->instance_count,
		atom->start_index_location,
		atom->base_vertex_location,
		atom->start_instance_location);
}


}