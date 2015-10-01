#include "render_atom.h"

namespace ue {

// noimpl hash
unsigned lame_hash(const void *whateves, size_t size, unsigned prev_hash) { return 0; }


RenderAtomFactory::RenderAtomFactory(ID3D12Device *device)
	: _device(device)
{}

RenderAtomFactory::~RenderAtomFactory()
{
}

size_t RenderAtomFactory::compile_placed_render_atom(const InstancedRenderAtomDesc &desc, void *buffer, size_t size)
{
	size_t required_size = sizeof(InstancedRenderAtom);
	required_size += sizeof(ID3D12PipelineState*);
	required_size += sizeof(D3D12_INDEX_BUFFER_VIEW);
	required_size += sizeof(D3D12_VERTEX_BUFFER_VIEW) * desc.vertex_buffer_count;
	required_size += sizeof(D3D12_CPU_DESCRIPTOR_HANDLE) * (desc.cbv_count + desc.uav_count + desc.srv_count + desc.sampler_count);
	// check size

	InstancedRenderAtom *render_atom = reinterpret_cast<InstancedRenderAtom*>(buffer);
	memset(render_atom, 0, size);

	char *p = reinterpret_cast<char*>(buffer);
	p += sizeof(InstancedRenderAtom);

	auto pso = _resource_manager->lookup_resource(desc.pso);
	memcpy(p, &pso, sizeof(ID3D12PipelineState*));
	p += sizeof(ID3D12PipelineState*);
	auto index_buffer = _resource_manager->lookup_resource(desc.index_buffer);
	memcpy(p, index_buffer, sizeof(D3D12_INDEX_BUFFER_VIEW));
	p += sizeof(D3D12_INDEX_BUFFER_VIEW);
	for (unsigned i = 0; i < desc.vertex_buffer_count; ++i) {
		auto vertex_buffer = _resource_manager->lookup_resource(desc.vertex_buffers[i]);
		memcpy(p, vertex_buffer, sizeof(D3D12_VERTEX_BUFFER_VIEW));
		p += sizeof(D3D12_VERTEX_BUFFER_VIEW);
		render_atom->vertex_buffer_count++;
	}

	for (unsigned i = 0; i < desc.srv_count; ++i) {
		auto srv = _resource_manager->lookup_resource(desc.srvs[i]);
		memcpy(p, srv, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
		p += sizeof(D3D12_CPU_DESCRIPTOR_HANDLE);
		render_atom->srv_count++;
		render_atom->srv_hash = lame_hash(srv, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE), render_atom->srv_hash);
	}

	for (unsigned i = 0; i < desc.uav_count; ++i) {
		auto uav = _resource_manager->lookup_resource(desc.srvs[i]);
		memcpy(p, uav, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
		p += sizeof(D3D12_CPU_DESCRIPTOR_HANDLE);
		render_atom->uav_count++;
		render_atom->uav_hash = lame_hash(uav, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE), render_atom->uav_hash);
	}

	for (unsigned i = 0; i < desc.cbv_count; ++i) {
		auto cbv = _resource_manager->lookup_resource(desc.srvs[i]);
		memcpy(p, cbv, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
		p += sizeof(D3D12_CPU_DESCRIPTOR_HANDLE);
		render_atom->cbv_count++;
		render_atom->cbv_hash = lame_hash(cbv, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE), render_atom->cbv_hash);
	}

	for (unsigned i = 0; i < desc.sampler_count; ++i) {
		auto sampler = _resource_manager->lookup_resource(desc.samplers[i]);
		memcpy(p, sampler, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
		p += sizeof(D3D12_CPU_DESCRIPTOR_HANDLE);
		render_atom->sampler_count++;
		render_atom->sampler_hash = lame_hash(sampler, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE), render_atom->sampler_hash);
	}


	return 0;
}


}