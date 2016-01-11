#include "application.h"

#include <core/platform/platform.h>
#include <core/thread/thread.h>
#include <core/thread/fiber_system.h>

#include <renderer_d3d12\renderer\d3d12_render_device.h>
#include <renderer_d3d12\renderer\command_list_factory.h>
#include <renderer_d3d12\renderer\online_descriptor_heap.h>
#include <renderer\primitive\primitive_factory.h>

#include <renderer_d3d12/renderer/render_atom.h>
#include <renderer_d3d12/renderer/render_tasks.h>

#include <core\memory\buddy_allocator_ext.h>

#include <math/vector3.h>
#include <math/vector4.h>
#include <math/matrix4.h>
#include <math/matrix3.h>

#include <renderer_d3d12/resource/d3d12_shader_factory.h>
#include <renderer_d3d12/resource/d3d12_resource_manager.h>

#include <renderer_d3d12/renderer/d3d12_command_list_factory.h>

#include <renderer_d3d12/memory/d3d12_memory.h>

namespace ue
{

Application::Application(void* window_handle)
: _window_handle(window_handle)
{}

Application::~Application()
{
	//delete _render_device;
}

void Application::initialize()
{
	//RenderDeviceDesc device_desc;
	//device_desc.window_handle = _window_handle;
	//device_desc.debug = true;
	//device_desc.back_buffer_count = 2;
	//device_desc.width = 1024;
	//device_desc.height = 768;
	//device_desc.fullscreen = false;
	//
	//auto primitives = new PrimitiveFactory();

	//_render_device = new D3D12RenderDevice(device_desc);
	//
	//d3d12_memory::initialize(_render_device->device());

	//
	//_command_list_factory = new D3D12CommandListFactory(*_render_device);
	//_descriptor_heap_factory = new OnlineDescriptorHeapFactory();


	//D3D12CommandListFactory::thread_initialize(_command_list_factory);

	//OnlineDescriptorHeapFactory::thread_initialize(_render_device->device());

	//D3D12ShaderFactory* shader_factory = new D3D12ShaderFactory(*_render_device);
	//D3D12ResourceManager* resource_manager = new D3D12ResourceManager(*_render_device);
	//_resource_manager = resource_manager;


	//const char* shader =
	//	R"(
	//	#define RS1 "CBV(b0), " \
	//				"DescriptorTable( CBV(b1), " \
	//					"SRV(t0, numDescriptors = 1)) "
	//	
	//	struct VS_OUT
	//	{
	//		float4 position : SV_Position;
	//		float2 texcoord : TEXCOORD0;
	//	};
	//	
	//	VS_OUT vs_main(uint id : SV_VertexID)
	//	{
	//		VS_OUT vs_out;
	//		float2 texcoord;
	//		texcoord.x = (id == 2) ? 2.0 : 0.0;
	//		texcoord.y = (id == 1) ? 2.0 : 0.0;
	//		vs_out.position = float4(texcoord * float2(2.0, -2.0) + float2(-1.0, 1.0), 1.0, 1.0);
	//		vs_out.texcoord = texcoord;
	//		return vs_out;
	//	}

	//	
	//	float4 ps_main(VS_OUT ps_in) : SV_Target
	//	{
	//		return float4(ps_in.texcoord, 1.0, 1.0);
	//	}

	//	)";
	//
	//auto vertex_shader = shader_factory->compile_shader_from_source(shader, "vs_main", "vs_5_0", 0);
	//auto pixel_shader = shader_factory->compile_shader_from_source(shader, "ps_main", "ps_5_0", 0);
	//auto root = shader_factory->compile_root_signature_from_source(shader, "RS1", 0);
	//root_signature_handle = resource_manager->create_root_signature(root);
	//
	//back_buffers[0] = resource_manager->create_render_target_from_resource(_render_device->get_back_buffer(0), D3D12_RESOURCE_STATE_PRESENT);
	//back_buffers[1] = resource_manager->create_render_target_from_resource(_render_device->get_back_buffer(1), D3D12_RESOURCE_STATE_PRESENT);

	//D3D12GraphicsPipelineStateDesc desc = {};
	//desc.root_signature = *reinterpret_cast<D3D12RootSignature*>(resource_manager->get_root_signature(root_signature_handle));
	//desc.shaders[VertexShader] = vertex_shader;
	//desc.shaders[PixelShader] = pixel_shader;
	//desc.primitive_topology_type = (unsigned)D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//desc.render_target_count = 1;
	//desc.render_target_formats[0] = reinterpret_cast<D3D12RenderTarget*>(resource_manager->get_resource(back_buffers[0]))->format;
	//desc.depth_stencil_format = (unsigned)DXGI_FORMAT_UNKNOWN;
	//pipeline_state_handle = resource_manager->create_graphics_pipeline_state(desc);



	//D3D12_VIEWPORT vp = {
	//	0, 0,
	//	device_desc.width,
	//	device_desc.height,
	//	0, 1
	//};

	//D3D12_RECT scissor = {
	//	0, 0, 
	//	device_desc.width,
	//	device_desc.height
	//};

	//viewport = vp;
	//scissor_rect = scissor;

	//float vb_verts[100];
	//unsigned short ib_indices[100];
	//unsigned n_verts;
	//unsigned n_indices;

	//primitives->cube(vb_verts, n_verts, ib_indices, n_indices, 0);

	//D3D12VertexBufferDesc vbd = {};
	//vbd.size = sizeof(Vector3)*n_verts;
	//vbd.stride = sizeof(Vector3);
	//auto vb_handle = _resource_manager->create_vertex_buffer(vbd, true);
	//auto vertex_buffer = (D3D12VertexBuffer*)_resource_manager->get_resource(vb_handle);
	//
	//D3D12IndexBufferDesc ibd = {};
	//ibd.size = sizeof(unsigned short) * n_indices;
	//ibd.format = DXGI_FORMAT_R16_UINT;
	//auto ib_handle = _resource_manager->create_index_buffer(ibd, true);
	//auto index_buffer = (D3D12IndexBuffer*)_resource_manager->get_resource(ib_handle);

	//auto command_list = _command_list_factory->get_graphics_command_list(0);

	//{
	//	D3D12BufferWriter writer;
	//	_resource_manager->buffer_writer(&vertex_buffer->buffer, &writer);
	//	writer.write(vb_verts, n_verts*sizeof(Vector3), command_list);
	//}
	//{
	//	D3D12BufferWriter writer;
	//	_resource_manager->buffer_writer(&index_buffer->buffer, &writer);
	//	writer.write(ib_indices, n_indices*sizeof(unsigned short), command_list);
	//}

	//command_list()->Close();
	//ID3D12CommandList* l[] = { command_list() };
	//_render_device->command_queue()->ExecuteCommandLists(1, l);

	//BufferWriter writer(vertex_buffer->buffer);
	//writer->command_list_write(vb_verts, 0, sizeof(Vector3)*n_verts, command_list);
		

	//RenderHandle pso_handle = _resource_manager->create_pipeline_state_object(PipelineStateObjectDesc());
	//RenderHandle vb_handle;
	//RenderHandle ib_handle;


	//auto &rrc = D3D12RenderResourceContext(*_render_device);
	//
	//back_buffers[0] = _resource_manager->create_render_target_from_resource(_render_device->get_back_buffer(0));
	//back_buffers[1] = _resource_manager->create_render_target_from_resource(_render_device->get_back_buffer(1));

	//{
	//	
	//	BufferDesc desc = {};
	//	desc.size = n_verts * sizeof(float) * 3;
	//	desc.stride = sizeof(float) * 3;
	//	desc.dynamic = false;
	//	vb_handle = _resource_manager->create_vertex_buffer(desc, vb_verts, rrc);
	//}

	//{
	//	BufferDesc desc = {};
	//	desc.size = n_indices * sizeof(unsigned short);
	//	desc.format = DXGI_FORMAT_R16_UINT;
	//	desc.dynamic = false;
	//	ib_handle = _resource_manager->create_index_buffer(desc, ib_indices, rrc);
	//}
	//	Matrix4x4 proj = perspectiveFovRH(60.f, device_desc.width / (float)device_desc.height, 0.1f, 100.f);
	//	Matrix4x4 view = lookAtRH(vector3(0, -3, 6), vector3(0, 0, 0), vector3(0, 1, 0));
	//	Matrix4x4 mvp = transpose(view * proj);

	//{
	//	BufferDesc desc = {};
	//	desc.size = sizeof(Matrix4x4);
	//	cb_handle = _resource_manager->create_constant_buffer(desc, &mvp, rrc);
	//	auto cbuffer = (ConstantBufferResource*)_resource_manager->lookup_resource(cb_handle);
	//	memcpy(cbuffer->mapped_data, &mvp, sizeof(Matrix4x4));
	//}
	//{
	//	RenderTargetDesc desc = {};
	//	desc.width = device_desc.width;
	//	desc.height = device_desc.height;
	//	desc.format = 1;
	//	desc.depth_stencil = true;
	//	depth_stencil_target = _resource_manager->create_render_target(desc, rrc);
	//}

	//{
	//	InstancedRenderAtomDesc desc = {};
	//	desc.pso = pso_handle;
	//	desc.vertex_buffers[0] = vb_handle;
	//	desc.vertex_buffer_count = 1;
	//	desc.index_buffer = ib_handle;
	//	desc.index_count_per_instance = n_indices;
	//	desc.base_vertex_location = 0;
	//	desc.instance_count = 1;
	//	desc.start_index_location = 0;
	//	desc.start_instance_location = 0;
	//	desc.topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	//	RootParameterDesc root_params[] =
	//	{
	//		{0, RootParameter::ROOT_CBV, 0 },
	//		{1, RootParameter::ROOT_DESCRIPTOR_TABLE, 0 },
	//		{1, RootParameter::ROOT_DESCRIPTOR_TABLE, 1 },
	//		{1, RootParameter::ROOT_DESCRIPTOR_TABLE, 2 },
	//		{2, RootParameter::ROOT_MULTIPLE_CONSTANTS, {4, 0} },
	//	};

	//	float root_constant_data = 98.f;
	//	Vector4 color = vector4(1, 0, 1, 1);
	//	RootParameterValue root_values[5] = {};
	//	root_values[0].render_resource = cb_handle;
	//	root_values[1].render_resource = cb_handle;
	//	root_values[2].render_resource = cb_handle;
	//	root_values[3].render_resource = cb_handle;
	//	root_values[4].constant_data = &color;

	//	render_atom = _resource_manager->create_render_atom(desc, 5, root_params, root_values);
	//}

	//auto rrc_command_list = _command_list_factory->aquire_command_list();
	//rrc.dispatch(*rrc_command_list);
	//
	//_command_list_factory->release_command_list(rrc_command_list);
	//rrc_command_list->submit(_render_device->command_queue());

	//_render_device->wait_for_fence();

	//test_task.device = _render_device->device();
	//test_task.root_signature = _resource_manager->root_signatire();
	//test_task.command_list_factory = _command_list_factory;
	//test_task.desctriptor_heap_factory = _descriptor_heap_factory;
	//test_task.task_index = 0;
	//test_task.atom_count = 1;
	//test_task.atom_offset = 0;
	//test_task.render_atoms.push_back((InstancedRenderAtom*)_resource_manager->lookup_resource(render_atom));
	//test_task.output_command_lists.resize(1);

}

Matrix4x4 _model = identity4x4();

struct D3D12FrameContext
{
	unsigned frame_index;
};



int Application::run()
{

	//if(!run_tests(_window_handle))
	//	return 1;

	//unsigned frame_index = _render_device->current_back_buffer();
	//_command_list_factory->reset_command_allocators(frame_index);


	//D3D12RenderTarget* rt = (D3D12RenderTarget*)_resource_manager->get_resource(back_buffers[frame_index]);
	//D3D12PipelineState* pipeline_state = (D3D12PipelineState*)_resource_manager->get_pipeline_state(pipeline_state_handle);

	//D3D12GraphicsCommandList& command_list = _command_list_factory->get_graphics_command_list(frame_index, pipeline_state->pipeline_state.Get());
	//
	//// TODO, make something cool for transitions
	//command_list()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(rt->resource.Get(), (D3D12_RESOURCE_STATES)rt->resource_state, D3D12_RESOURCE_STATE_RENDER_TARGET));
	//rt->resource_state = D3D12_RESOURCE_STATE_RENDER_TARGET;

	//auto render_target_desc_handle = _resource_manager->get_render_target_descriptor_handle(rt, D3D12RenderTarget::RTV_DESCRIPTOR);

	//const float clear[] = {0.2f, 0.2f, 1.0f, 0.0f};
	//command_list()->ClearRenderTargetView(render_target_desc_handle, clear, 0, nullptr);
	//
	//command_list()->RSSetViewports(1, &viewport);
	//command_list()->RSSetScissorRects(1, &scissor_rect);
	//command_list()->SetGraphicsRootSignature(pipeline_state->root_signature.signature.Get());

	//command_list()->OMSetRenderTargets(1, &render_target_desc_handle, false, nullptr);
	//command_list()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//command_list()->DrawInstanced(3, 1, 0, 0);

	//command_list()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(rt->resource.Get(), (D3D12_RESOURCE_STATES)rt->resource_state, D3D12_RESOURCE_STATE_PRESENT));
	//rt->resource_state = D3D12_RESOURCE_STATE_PRESENT;
	//command_list()->Close();
	//
	//

	//ID3D12CommandList* lists[] = { command_list() };
	//_render_device->command_queue()->ExecuteCommandLists(1, lists);

	//_command_list_factory->executed(command_list);


	//_render_device->present();
	//_render_device->wait_for_fence();

	//
	//Matrix4x4 rotation = matrix4x4(rotationX(0.02f)) * matrix4x4(rotationY(0.01f));
	//Matrix4x4 proj = perspectiveFovRH(60.f, 1024.f / (float)768.f, 0.1f, 100.f);
	//Matrix4x4 view = lookAtRH(vector3(0, 0, -6), vector3(0, 0, 0), vector3(0, 1, 0));
	//_model = rotation * _model;
	//Matrix4x4 mvp = transpose(_model * view * proj);

	//ConstantBufferResource *cb = (ConstantBufferResource*)_resource_manager->lookup_resource(cb_handle);
	//memcpy(cb->map(0), &mvp, sizeof(Matrix4x4));

	//auto cmd_list = _command_list_factory->aquire_command_list();
	//auto command_list = cmd_list->command_list();

	//D3D12_VIEWPORT viewport = 
	//{
	//	0, 0,
	//	1024, 768,
	//	0, 1
	//};

	//D3D12_RECT scissor = 
	//{
	//	0, 0,
	//	1024, 768
	//};

	//RenderTargetResource *rt = (RenderTargetResource*)_resource_manager->lookup_resource(back_buffers[_render_device->current_back_buffer()]);
	//RenderTargetResource *dst = (RenderTargetResource*)_resource_manager->lookup_resource(depth_stencil_target);

	//rt->transition(command_list, RenderTargetResource::RENDER_TARGET);


	//const float clear[] = { 0.f,0.2f,0.4f,1.f };
	//command_list->ClearRenderTargetView(*rt->rtv, clear, 0, nullptr);
	//command_list->ClearDepthStencilView(*dst->dsv, D3D12_CLEAR_FLAG_DEPTH, 1, 0, 0, nullptr);
	//_command_list_factory->release_command_list(cmd_list);
	//cmd_list->submit(_render_device->command_queue());

	//test_task.rt = rt;
	//test_task.dst = dst;
	//test_task.vp = viewport;
	//test_task.scissor = scissor;
	//test_task.override_cmd_list = nullptr;

	//DrawInstancedTask::execute(&test_task);
	//test_task.output_command_lists[0]->submit(_render_device->command_queue());

	//cmd_list = _command_list_factory->aquire_command_list();
	//command_list = cmd_list->command_list();

	//rt->transition(command_list, RenderTargetResource::PRESENT);

	//_command_list_factory->release_command_list(cmd_list);
	//cmd_list->submit(_render_device->command_queue());

	//_render_device->wait_for_fence();
	//_render_device->present();

	////FiberSystemParams* fiber_system_params = new FiberSystemParams();
	////FiberSystem &fiber_system = fiber_system_params->_fiber_system;

	////fiber_system.initialize(fiber_system_params, platform::hardware_thread_count(), 1024);
	//////fiber_system.wait_for_counter(counter, 0);

	////fiber_system.quit();


	return 0;
}

}