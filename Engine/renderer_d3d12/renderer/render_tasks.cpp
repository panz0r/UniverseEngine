#include "render_tasks.h"
#include "render_atom.h"
#include "command_list_factory.h"
#include "descriptor_heap_factory.h"

#include <d3d12.h>

namespace ue
{

struct DrawInstancedTaskData
{
	ID3D12Device *device;
	ID3D12RootSignature *root_signature;
	CommandListFactory *command_list_factory;
	DescriptorHeapFactory *desctriptor_heap_factory;

	unsigned task_index;
	unsigned atom_count;
	unsigned atom_offset;
	std::vector<InstancedRenderAtom*> *render_atoms;
	std::vector<CommandListInterface<ID3D12GraphicsCommandList>*> output_command_lists;
	
};

void DrawInstancedTask::execute(void *user_data) {

	auto task_data = reinterpret_cast<DrawInstancedTaskData*>(user_data);

	ID3D12CommandQueue *command_queue = nullptr;
	StateCache state_cache = {};
	ID3D12Device *device = task_data->device;
	DescriptorHeaps descriptor_heaps = {};

	auto cbv_srv_uav_heap = task_data->desctriptor_heap_factory->checkout_descriptor_heap(DescriptorHeapFactory::CBV_UAV_SRV);
	auto sampler_heap = task_data->desctriptor_heap_factory->checkout_descriptor_heap(DescriptorHeapFactory::Sampler);
	
	auto command_list_interface = task_data->command_list_factory->aquire_command_list<ID3D12GraphicsCommandList>();
	auto command_list = command_list_interface->_command_list.Get();
	
	auto &render_atoms = *task_data->render_atoms;

	unsigned atom_count = task_data->atom_count;
	unsigned atom_offset = task_data->atom_offset;

	command_list->SetGraphicsRootSignature(task_data->root_signature);
	ID3D12DescriptorHeap *heaps[] = { cbv_srv_uav_heap->_heap.Get(), sampler_heap->_heap.Get() };
	command_list->SetDescriptorHeaps(_countof(heaps), heaps);


	for (unsigned i = atom_offset; i < atom_count; ++i) {
		draw_instanced_render_atom(render_atoms[i], command_list, device, &descriptor_heaps, &state_cache);
	}

	command_list->Close();
	task_data->output_command_lists[task_data->task_index] = command_list_interface;
	
	// must be released back on this thread since it was aquired from this thread
	// It's not really made availalble for reuse until it has been signalled as submitted
	task_data->command_list_factory->release_command_list(command_queue, command_list_interface);

}


}