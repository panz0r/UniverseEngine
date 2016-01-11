#include "render_tasks.h"
#include "render_atom.h"
#include "command_list_factory.h"
#include "online_descriptor_heap.h"
#include <renderer_d3d12/resource/d3d12_texture.h>
#include <d3d12.h>

namespace ue
{


void DrawInstancedTask::execute(void *user_data) {

	auto task_data = reinterpret_cast<DrawInstancedTaskData*>(user_data);

	StateCache state_cache = {};
	ID3D12Device *device = task_data->device;

	OnlineDescriptorHeap *cbv_srv_uav_heap = task_data->desctriptor_heap_factory->checkout_descriptor_heap(OnlineDescriptorHeapFactory::CBV_UAV_SRV);
	OnlineDescriptorHeap *sampler_heap = task_data->desctriptor_heap_factory->checkout_descriptor_heap(OnlineDescriptorHeapFactory::Sampler);
	DescriptorHeaps descriptor_heaps = { cbv_srv_uav_heap, sampler_heap };
	
	ID3D12GraphicsCommandList *command_list;

	auto command_list_interface = task_data->command_list_factory->aquire_command_list();
	command_list = command_list_interface->command_list();
	
	auto &render_atoms = task_data->render_atoms;

	unsigned atom_count = task_data->atom_count;
	unsigned atom_offset = task_data->atom_offset;

	if (task_data->override_cmd_list != nullptr)
		command_list = task_data->override_cmd_list;

	command_list->SetGraphicsRootSignature(task_data->root_signature);
	
	command_list->OMSetRenderTargets(1, task_data->rt->rtv, false, task_data->dst->dsv);
	command_list->RSSetViewports(1, &task_data->vp);
	command_list->RSSetScissorRects(1, &task_data->scissor);

	ID3D12DescriptorHeap *heaps[] = { cbv_srv_uav_heap->_heap.Get(), sampler_heap->_heap.Get() };
	command_list->SetDescriptorHeaps(_countof(heaps), heaps);


	for (unsigned i = atom_offset; i < atom_count; ++i) {
		draw_instanced_render_atom(render_atoms[i], command_list, device, &descriptor_heaps, &state_cache);
	}

	//command_list->Close();
	task_data->output_command_lists[task_data->task_index] = command_list_interface;
	
	// must be released back on this thread since it was aquired from this thread
	// It's not really made availalble for reuse until it has been signalled as submitted
	task_data->command_list_factory->release_command_list(command_list_interface);

}


}