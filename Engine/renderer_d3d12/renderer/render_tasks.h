#pragma once

#include <core/platform/platform.h>
#include <d3d12.h>
#include <vector>
#include <renderer_d3d12/renderer/command_list_factory.h>

namespace ue
{

class D3D12RenderDevice;
class ResourceManager;
class CommandListFactory;
class OnlineDescriptorHeapFactory;
struct InstancedRenderAtom;
struct InstancedRenderAtom;
struct RenderTargetResource;

struct DrawInstancedTaskData
{
	ID3D12Device *device;
	ID3D12RootSignature *root_signature;
	CommandListFactory *command_list_factory;
	OnlineDescriptorHeapFactory *desctriptor_heap_factory;
	ID3D12GraphicsCommandList *override_cmd_list;


	RenderTargetResource* rt;
	RenderTargetResource* dst;
	D3D12_VIEWPORT vp;
	D3D12_RECT scissor;

	unsigned task_index;
	unsigned atom_count;
	unsigned atom_offset;
	std::vector<InstancedRenderAtom*> render_atoms;
	std::vector<GraphicsCommandList*> output_command_lists;
};

template <class T>
T* next_render_atom(T* render_atom) {
	return reinterpret_cast<T*>(reinterpret_cast<char*>(render_atom) + render_atom->size);
}


class DrawInstancedTask
{
public:
	static void execute(void *user_data);

};

}