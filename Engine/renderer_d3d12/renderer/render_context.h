#pragma once

#include <d3d12.h>
#include <vector>

namespace ue
{


struct RenderBatch
{
	unsigned atom_count;
};

class ResourceManager;

class RenderContext
{
public:
	RenderContext(ID3D12GraphicsCommandList* command_list)
	: _root_signature(nullptr)
	, _pso(nullptr)
	, _command_list(command_list) 
	{}
	
	void render(class RenderAtom *atom, ID3D12GraphicsCommandList *command_list);

	ID3D12RootSignature*	_root_signature;
	ID3D12PipelineState*	_pso;
private:
	ID3D12Device*	_device;
	ID3D12GraphicsCommandList* _command_list;
	ID3D12DescriptorHeap** _runtime_heaps;

};

}