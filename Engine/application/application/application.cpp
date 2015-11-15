#include "application.h"

#include <core/platform/platform.h>
#include <core/thread/thread.h>
#include <core/thread/fiber_system.h>

#include <renderer_d3d12\renderer\render_device.h>
#include <renderer_d3d12\renderer\resource_manager.h>
#include <renderer_d3d12\renderer\command_list_factory.h>
#include <renderer_d3d12\primitive.h>

#include <renderer_d3d12/renderer/render_atom.h>
#include <renderer_d3d12/renderer/render_tasks.h>

#include <core\memory\buddy_allocator_ext.h>

#include <math/vector3.h>
#include <math/vector4.h>
#include <math/matrix4.h>

namespace ue
{

Application::Application(void* window_handle)
: _window_handle(window_handle)
{}

Application::~Application()
{
	delete _render_device;
}

RenderHandle render_atom;
RenderHandle back_buffers[2];

DrawInstancedTaskData test_task;


void Application::initialize()
{
	size_t buffer_size = 1024*1024*128;
	void *buffer = malloc(buffer_size);


	D3D12RenderDeviceDesc device_desc;
	device_desc.window_handle = (HWND)_window_handle;
	device_desc.debug = true;
	device_desc.back_buffer_count = 2;
	device_desc.width = 1024;
	device_desc.height = 768;
	device_desc.fullscreen = false;
	
	auto primitives = new PrimitiveFactory();

	_render_device = new D3D12RenderDevice(device_desc);
	_command_list_factory = new CommandListFactory(*_render_device);
	_resource_manager = new ResourceManager(*_render_device);
	_descriptor_heap_factory = new DescriptorHeapFactory();

	CommandListFactory::thread_initialize();
	DescriptorHeapFactory::thread_initialize(_render_device->device());


	RenderHandle pso_handle = _resource_manager->create_pipeline_state_object(PipelineStateObjectDesc());
	RenderHandle vb_handle;
	RenderHandle ib_handle;
	RenderHandle cb_handle;


	auto &rrc = RenderResourceContext(*_render_device);
	
	float vb_verts[100];
	unsigned short ib_indices[100];
	unsigned n_verts;
	unsigned n_indices;

	primitives->cube(vb_verts, n_verts, ib_indices, n_indices, 0);

	back_buffers[0] = _resource_manager->create_render_target_from_resource(_render_device->back_buffer(0));
	back_buffers[1] = _resource_manager->create_render_target_from_resource(_render_device->back_buffer(1));

	{
		
		BufferDesc desc = {};
		desc.size = n_verts * sizeof(float) * 3;
		desc.stride = sizeof(float) * 3;
		desc.dynamic = false;
		vb_handle = _resource_manager->create_vertex_buffer(desc, vb_verts, rrc);
	}

	{
		BufferDesc desc = {};
		desc.size = n_indices * sizeof(unsigned short);
		desc.format = DXGI_FORMAT_R16_UINT;
		desc.dynamic = false;
		ib_handle = _resource_manager->create_index_buffer(desc, ib_indices, rrc);
	}
		Matrix4x4 proj = perspectiveFovRH(60.f, device_desc.width / (float)device_desc.height, 0.1f, 100.f);
		Matrix4x4 view = lookAtRH(vector3(0, -3, 6), vector3(0, 0, 0), vector3(0, 1, 0));
		Matrix4x4 mvp = transpose(view * proj);

	{
		BufferDesc desc = {};
		desc.size = sizeof(Matrix4x4);
		cb_handle = _resource_manager->create_constant_buffer(desc, &mvp, rrc);
	}

	{
		InstancedRenderAtomDesc desc = {};
		desc.pso = pso_handle;
		desc.vertex_buffers[0] = vb_handle;
		desc.vertex_buffer_count = 1;
		desc.index_buffer = ib_handle;
		desc.index_count_per_instance = n_indices;
		desc.base_vertex_location = 0;
		desc.instance_count = 1;
		desc.start_index_location = 0;
		desc.start_instance_location = 0;
		desc.topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		desc.cbv_count = 1;
		desc.cbvs[0] = cb_handle;
		
		render_atom = _resource_manager->create_render_atom(desc);
	}

	auto rrc_command_list = _command_list_factory->aquire_command_list<ID3D12GraphicsCommandList>();
	rrc.dispatch(*rrc_command_list);
	
	_command_list_factory->release_command_list(rrc_command_list);
	rrc_command_list->submit(_render_device->command_queue());

	_render_device->wait_for_fence();

	test_task.device = _render_device->device();
	test_task.root_signature = _resource_manager->root_signatire();
	test_task.command_list_factory = _command_list_factory;
	test_task.desctriptor_heap_factory = _descriptor_heap_factory;
	test_task.task_index = 0;
	test_task.atom_count = 2;
	test_task.atom_offset = 0;
	test_task.render_atoms.push_back((InstancedRenderAtom*)_resource_manager->lookup_resource(render_atom));
	test_task.render_atoms.push_back((InstancedRenderAtom*)_resource_manager->lookup_resource(render_atom));
	test_task.output_command_lists.resize(1);



	//TextureDesc tex_desc;
	//tex_desc.width = 256;
	//tex_desc.height = 256;
	//tex_desc.depth = 1;
	//tex_desc.dimension = 3;
	//tex_desc.format = 0;
	//tex_desc.mip_levels = 1;
	//tex_desc.dynamic = false;

	//auto texture = resource_manager->create_texture(resource_command_list, tex_desc, nullptr);
	//auto texture2 = resource_manager->create_texture(resource_command_list, tex_desc, nullptr);
	//resource_manager->destroy_texture(texture);
	//resource_manager->destroy_texture(texture2);

	//D3D11RenderDevice* device = new D3D11RenderDevice((HWND)_window_handle, 1024, 768);
	//D3D11RenderContext* context = new D3D11RenderContext(device->context());

	//unsigned states = rand();
	//context->drawcall(states);
}


int Application::run()
{

//	InstancedRenderAtom *atom = (InstancedRenderAtom*)_resource_manager->lookup_resource(render_atom);

	auto cmd_list = _command_list_factory->aquire_command_list<ID3D12GraphicsCommandList>();
	auto command_list = cmd_list->command_list();

//	command_list->SetGraphicsRootSignature(_resource_manager->root_signatire());


	D3D12_VIEWPORT viewport = 
	{
		0, 0,
		1024, 768,
		0, 1
	};

	D3D12_RECT scissor = 
	{
		0, 0,
		1024, 768
	};

	command_list->RSSetViewports(1, &viewport);
	command_list->RSSetScissorRects(1, &scissor);

	RenderTargetResource *rt = (RenderTargetResource*)_resource_manager->lookup_resource(back_buffers[_render_device->current_back_buffer()]);
	rt->transition(command_list, RenderTargetResource::RENDER_TARGET);

	//command_list->OMSetRenderTargets(1, rt->rtv, false, nullptr);

	const float clear[] = { 0.f,0.2f,0.4f,1.f };
	command_list->ClearRenderTargetView(*rt->rtv, clear, 0, nullptr);

	_command_list_factory->release_command_list(cmd_list);
	cmd_list->submit(_render_device->command_queue());

	test_task.rt = rt;
	test_task.vp = viewport;
	test_task.scissor = scissor;
	test_task.override_cmd_list = nullptr;

	DrawInstancedTask::execute(&test_task);
	test_task.output_command_lists[0]->submit(_render_device->command_queue());



	cmd_list = _command_list_factory->aquire_command_list<ID3D12GraphicsCommandList>();
	command_list = cmd_list->command_list();

	rt->transition(command_list, RenderTargetResource::PRESENT);

	_command_list_factory->release_command_list(cmd_list);
	cmd_list->submit(_render_device->command_queue());

	_render_device->present();

	_render_device->wait_for_fence();


	//FiberSystemParams* fiber_system_params = new FiberSystemParams();
	//FiberSystem &fiber_system = fiber_system_params->_fiber_system;

	//fiber_system.initialize(fiber_system_params, platform::hardware_thread_count(), 1024);
	////fiber_system.wait_for_counter(counter, 0);

	//fiber_system.quit();


	return 0;
}

}