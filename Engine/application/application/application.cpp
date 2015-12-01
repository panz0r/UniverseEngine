#include "application.h"

#include <core/platform/platform.h>
#include <core/thread/thread.h>
#include <core/thread/fiber_system.h>

#include <renderer_d3d12\renderer\d3d12_render_device.h>
#include <renderer_d3d12\renderer\resource_manager.h>
#include <renderer_d3d12\renderer\command_list_factory.h>
#include <renderer\primitive\primitive_factory.h>

#include <renderer_d3d12/renderer/render_atom.h>
#include <renderer_d3d12/renderer/render_tasks.h>

#include <core\memory\buddy_allocator_ext.h>

#include <renderer_d3d12/resource/d3d12_constant_buffer.h>

#include <math/vector3.h>
#include <math/vector4.h>
#include <math/matrix4.h>
#include <math/matrix3.h>

#include <renderer_d3d12/memory/d3d12_allocator.h>

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
RenderHandle depth_stencil_target;
RenderHandle cb_handle;

DrawInstancedTaskData test_task;


void Application::initialize()
{
	size_t buffer_size = 1024*1024*128;
	void *buffer = malloc(buffer_size);


	RenderDeviceDesc device_desc;
	device_desc.window_handle = _window_handle;
	device_desc.debug = true;
	device_desc.back_buffer_count = 2;
	device_desc.width = 1024;
	device_desc.height = 768;
	device_desc.fullscreen = false;
	
	auto primitives = new PrimitiveFactory();

	_render_device = new D3D12RenderDevice(device_desc);
	_command_list_factory = new CommandListFactory(*_render_device);
	_resource_manager = new ResourceManager(*_render_device);
	_descriptor_heap_factory = new OnlineDescriptorHeapFactory();

	CommandListFactory::thread_initialize();
	OnlineDescriptorHeapFactory::thread_initialize(_render_device->device());

	D3D12DescriptorAllocatorDesc descriptor_allocator_desc;
	descriptor_allocator_desc.size = 8;
	descriptor_allocator_desc.flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	descriptor_allocator_desc.type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	D3D12DescriptorAllocator *offline = new D3D12DescriptorAllocator(_render_device->device(), descriptor_allocator_desc);

	auto handle1 = offline->allocate(1);
	auto handle2 = offline->allocate(2);
	auto handle3 = offline->allocate(2);
	auto handle32 = offline->allocate(2);
	auto handle4 = offline->allocate(7);
	auto handle5 = offline->allocate(1);
	auto handle6 = offline->allocate(2);
	
	offline->deallocate(handle1);
	offline->deallocate(handle3);
	offline->deallocate(handle4);
	offline->deallocate(handle2);

	auto test = offline->get_cpu_handle(handle32);




	RenderHandle pso_handle = _resource_manager->create_pipeline_state_object(PipelineStateObjectDesc());
	RenderHandle vb_handle;
	RenderHandle ib_handle;


	auto &rrc = RenderResourceContext(*_render_device);
	
	float vb_verts[100];
	unsigned short ib_indices[100];
	unsigned n_verts;
	unsigned n_indices;

	primitives->cube(vb_verts, n_verts, ib_indices, n_indices, 0);

	back_buffers[0] = _resource_manager->create_render_target_from_resource(_render_device->get_back_buffer(0));
	back_buffers[1] = _resource_manager->create_render_target_from_resource(_render_device->get_back_buffer(1));

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
		auto cbuffer = (ConstantBufferResource*)_resource_manager->lookup_resource(cb_handle);
		memcpy(cbuffer->mapped_data, &mvp, sizeof(Matrix4x4));
	}
	{
		RenderTargetDesc desc = {};
		desc.width = device_desc.width;
		desc.height = device_desc.height;
		desc.format = 1;
		desc.depth_stencil = true;
		depth_stencil_target = _resource_manager->create_render_target(desc, rrc);
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

		RootParameterDesc root_params[] =
		{
			{0, RootParameter::ROOT_CBV, 0 },
			{1, RootParameter::ROOT_DESCRIPTOR_TABLE, 0 },
			{1, RootParameter::ROOT_DESCRIPTOR_TABLE, 1 },
			{1, RootParameter::ROOT_DESCRIPTOR_TABLE, 2 },
			{2, RootParameter::ROOT_MULTIPLE_CONSTANTS, {4, 0} },
		};

		float root_constant_data = 98.f;
		Vector4 color = vector4(1, 0, 1, 1);
		RootParameterValue root_values[5] = {};
		root_values[0].render_resource = cb_handle;
		root_values[1].render_resource = cb_handle;
		root_values[2].render_resource = cb_handle;
		root_values[3].render_resource = cb_handle;
		root_values[4].constant_data = &color;

		render_atom = _resource_manager->create_render_atom(desc, 5, root_params, root_values);
	}

	auto rrc_command_list = _command_list_factory->aquire_command_list();
	rrc.dispatch(*rrc_command_list);
	
	_command_list_factory->release_command_list(rrc_command_list);
	rrc_command_list->submit(_render_device->command_queue());

	_render_device->wait_for_fence();

	test_task.device = _render_device->device();
	test_task.root_signature = _resource_manager->root_signatire();
	test_task.command_list_factory = _command_list_factory;
	test_task.desctriptor_heap_factory = _descriptor_heap_factory;
	test_task.task_index = 0;
	test_task.atom_count = 1;
	test_task.atom_offset = 0;
	test_task.render_atoms.push_back((InstancedRenderAtom*)_resource_manager->lookup_resource(render_atom));
	test_task.output_command_lists.resize(1);

}

Matrix4x4 _model = identity4x4();

int Application::run()
{

	
	Matrix4x4 rotation = matrix4x4(rotationX(0.02f)) * matrix4x4(rotationY(0.01f));
	Matrix4x4 proj = perspectiveFovRH(60.f, 1024.f / (float)768.f, 0.1f, 100.f);
	Matrix4x4 view = lookAtRH(vector3(0, 0, -6), vector3(0, 0, 0), vector3(0, 1, 0));
	_model = rotation * _model;
	Matrix4x4 mvp = transpose(_model * view * proj);

	ConstantBufferResource *cb = (ConstantBufferResource*)_resource_manager->lookup_resource(cb_handle);
	memcpy(cb->map(0), &mvp, sizeof(Matrix4x4));

	auto cmd_list = _command_list_factory->aquire_command_list();
	auto command_list = cmd_list->command_list();

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

	RenderTargetResource *rt = (RenderTargetResource*)_resource_manager->lookup_resource(back_buffers[_render_device->current_back_buffer()]);
	RenderTargetResource *dst = (RenderTargetResource*)_resource_manager->lookup_resource(depth_stencil_target);

	rt->transition(command_list, RenderTargetResource::RENDER_TARGET);


	const float clear[] = { 0.f,0.2f,0.4f,1.f };
	command_list->ClearRenderTargetView(*rt->rtv, clear, 0, nullptr);
	command_list->ClearDepthStencilView(*dst->dsv, D3D12_CLEAR_FLAG_DEPTH, 1, 0, 0, nullptr);
	_command_list_factory->release_command_list(cmd_list);
	cmd_list->submit(_render_device->command_queue());

	test_task.rt = rt;
	test_task.dst = dst;
	test_task.vp = viewport;
	test_task.scissor = scissor;
	test_task.override_cmd_list = nullptr;

	DrawInstancedTask::execute(&test_task);
	test_task.output_command_lists[0]->submit(_render_device->command_queue());

	cmd_list = _command_list_factory->aquire_command_list();
	command_list = cmd_list->command_list();

	rt->transition(command_list, RenderTargetResource::PRESENT);

	_command_list_factory->release_command_list(cmd_list);
	cmd_list->submit(_render_device->command_queue());

	_render_device->wait_for_fence();
	_render_device->present();



	//FiberSystemParams* fiber_system_params = new FiberSystemParams();
	//FiberSystem &fiber_system = fiber_system_params->_fiber_system;

	//fiber_system.initialize(fiber_system_params, platform::hardware_thread_count(), 1024);
	////fiber_system.wait_for_counter(counter, 0);

	//fiber_system.quit();


	return 0;
}

}