#pragma once

#include "test_base.h"

#include <renderer/primitive/primitive_factory.h>
#include <renderer_d3d12/renderer/d3d12_render_device.h>
#include <renderer_d3d12/memory/d3d12_memory.h>
#include <renderer_d3d12/resource/d3d12_shader_factory.h>
#include <renderer_d3d12/resource/d3d12_resource_manager.h>
#include <renderer_d3d12/renderer/d3d12_command_list_factory.h>
#include <renderer_d3d12/renderer/d3d12_render_resource_context.h>
#include <renderer_d3d12/stdinclude.h>

#include <math/math.h>
#include <math/vector3.h>
#include <math/matrix4.h>
#include <math/matrix3.h>

namespace ue
{

/////////////////////////////////////////

class BasicObjectTest : public UnitTestBase
{
public:
	D3D12RenderDevice* _render_device;
	D3D12ShaderFactory* _shader_factory;

	RenderHandle back_buffers[2];
	D3D12_VIEWPORT viewport;
	D3D12_RECT scissor_rect;
	RenderSetHandle pipeline_state_handle;
	RenderSetHandle root_signature_handle;

	RenderHandle vb_handle, ib_handle;
	unsigned n_verts, n_indices;

	/////////////////////////////////////////

	BasicObjectTest(unsigned width, unsigned height, const char* name)
		: UnitTestBase(width, height, name)
	{
		RenderDeviceDesc device_desc;
		device_desc.window_handle = _hWnd;
		device_desc.debug = true;
		device_desc.back_buffer_count = 2;
		device_desc.width = width;
		device_desc.height = height;
		device_desc.fullscreen = false;

		_render_device = new D3D12RenderDevice(device_desc);
		_shader_factory = new D3D12ShaderFactory(*_render_device);

		viewport.Width = device_desc.width;
		viewport.Height = device_desc.height;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;

		scissor_rect.top = 0;
		scissor_rect.left = 0;
		scissor_rect.right = device_desc.width;
		scissor_rect.bottom = device_desc.height;

		///////////////////////////////////////

		const char* shader =
			R"(
	#define RS1 "RootFlags( ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT) "
		
	struct VS_INPUT
	{
		float3 position : POSITION;
	};

	struct VS_OUT
	{
		float4 position : SV_Position;
	};
		
	VS_OUT vs_main(VS_INPUT input)
	{
		VS_OUT vs_out;
		vs_out.position = float4(input.position, 1);
		return vs_out;
	}
													
	float4 ps_main(VS_OUT ps_in) : SV_Target
	{
		return float4(1,0,0,1);
	}

			)";

		auto vertex_shader = _shader_factory->compile_shader_from_source(shader, "vs_main", "vs_5_0", 0);
		auto pixel_shader = _shader_factory->compile_shader_from_source(shader, "ps_main", "ps_5_0", 0);
		auto root_signature = _shader_factory->compile_root_signature_from_source(shader, "RS1", 0);
		auto root_signature_handle = _render_device->resource_manager()->create_root_signature(root_signature);

		back_buffers[0] = _render_device->resource_manager()->create_render_target_from_resource(_render_device->get_back_buffer(0), D3D12_RESOURCE_STATE_PRESENT);
		back_buffers[1] = _render_device->resource_manager()->create_render_target_from_resource(_render_device->get_back_buffer(1), D3D12_RESOURCE_STATE_PRESENT);

		D3D12GraphicsPipelineStateDesc desc = {};

		D3D12_INPUT_ELEMENT_DESC input_elements[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		};

		D3D12_INPUT_LAYOUT_DESC input_layout = {
			input_elements, 1
		};

		desc.input_layout = input_layout;
		desc.root_signature = *(_render_device->resource_manager()->root_signature(root_signature_handle));
		desc.shaders[VertexShader] = vertex_shader;
		desc.shaders[PixelShader] = pixel_shader;
		desc.primitive_topology_type = (unsigned)D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		desc.render_target_count = 1;
		desc.render_target_formats[0] = _render_device->resource_manager()->render_target(back_buffers[0])->format;
		desc.depth_stencil_format = (unsigned)DXGI_FORMAT_UNKNOWN;
		pipeline_state_handle = _render_device->resource_manager()->create_graphics_pipeline_state(desc);

		Vector3 verts[100];
		unsigned short indices[100];

		PrimitiveFactory primitive_factory;
		primitive_factory.cube((float*)verts, n_verts, indices, n_indices, Position);
		
		// proj and view matrix
		float aspect = width / (float)height;
		Matrix4x4 projection = perspectiveFovRH(Math::radians(90.f), aspect, 0.1f, 100.f);
		Matrix4x4 view = lookAtRH(vector3(20,30, 50), vector3(0,0,0), vector3(0,1,0));
		Matrix4x4 view_proj = (view * projection);

		// transform vertices
		for (unsigned i = 0; i < n_verts; ++i) {
			verts[i] = transformCoord(view_proj, verts[i]);
		}

		D3D12VertexBufferDesc vb_desc = {};
		vb_desc.size = sizeof(Vector3)*n_verts;
		vb_desc.stride = sizeof(Vector3);
		vb_handle = _render_device->resource_manager()->create_vertex_buffer(vb_desc, true);

		D3D12IndexBufferDesc ib_desc = {};
		ib_desc.format = (unsigned)DXGI_FORMAT_R16_UINT;
		ib_desc.size = sizeof(unsigned short)*n_indices;
		ib_handle = _render_device->resource_manager()->create_index_buffer(ib_desc, true);
		

		D3D12GraphicsCommandList& command_list = _render_device->command_list_factory()->get_graphics_command_list(0);

		{
			auto vbuffer = _render_device->resource_manager()->vertex_buffer(vb_handle);
			//rrc->update_buffer(vbuffer->buffer, void* data, size);

			command_list()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
				vbuffer->buffer.resource.resource.Get(),
				vbuffer->buffer.resource.resource_state, 
				D3D12_RESOURCE_STATE_COPY_DEST));
			
			D3D12BufferWriter writer(&vbuffer->buffer);
			writer.write(verts, sizeof(Vector3)*n_verts, command_list);

			command_list()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
				vbuffer->buffer.resource.resource.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST,
				vbuffer->buffer.resource.resource_state));
		}

		{
			auto ibuffer = _render_device->resource_manager()->index_buffer(ib_handle);
			command_list()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
				ibuffer->buffer.resource.resource.Get(),
				ibuffer->buffer.resource.resource_state,
				D3D12_RESOURCE_STATE_COPY_DEST));

			D3D12BufferWriter writer(&ibuffer->buffer);
			writer.write(indices, sizeof(unsigned short)*n_indices, command_list);

			command_list()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
				ibuffer->buffer.resource.resource.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST,
				ibuffer->buffer.resource.resource_state));
		}

		//render_device->dispatch(1, rrc);
		//render_device->destroy_rrc(rrc);

		command_list()->Close();

		ID3D12CommandList* rrc[] = { command_list() };
		_render_device->command_queue()->ExecuteCommandLists(1, rrc);
		_render_device->command_list_factory()->executed(command_list);

		

		_render_device->wait_for_fence();
	}

	virtual ~BasicObjectTest()
	{
		delete _shader_factory;
		delete _render_device;
	}

	virtual int execute_test()
	{
		unsigned frame_index = _render_device->current_back_buffer();
		_render_device->command_list_factory()->reset_command_allocators(frame_index);

		D3D12RenderTarget* rt = _render_device->resource_manager()->render_target(back_buffers[frame_index]);
		D3D12PipelineState* pipeline_state = _render_device->resource_manager()->pipeline_state(pipeline_state_handle);

		D3D12GraphicsCommandList& command_list = _render_device->command_list_factory()->get_graphics_command_list(frame_index, pipeline_state->pipeline_state.Get());

		command_list()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(rt->resource.Get(), (D3D12_RESOURCE_STATES)rt->resource_state, D3D12_RESOURCE_STATE_RENDER_TARGET));
		rt->resource_state = D3D12_RESOURCE_STATE_RENDER_TARGET;

		auto render_target_desc_handle = _render_device->resource_manager()->get_render_target_descriptor_handle(rt, D3D12RenderTarget::RTV_DESCRIPTOR);

		const float clear[] = { 0.2f, 0.2f, 1.0f, 0.0f };
		command_list()->ClearRenderTargetView(render_target_desc_handle, clear, 0, nullptr);

		command_list()->RSSetViewports(1, &viewport);
		command_list()->RSSetScissorRects(1, &scissor_rect);
		command_list()->SetGraphicsRootSignature(pipeline_state->root_signature.signature.Get());

		command_list()->OMSetRenderTargets(1, &render_target_desc_handle, false, nullptr);
		command_list()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		auto vbuffer = _render_device->resource_manager()->vertex_buffer(vb_handle);
		auto ibuffer = _render_device->resource_manager()->index_buffer(ib_handle);

		command_list()->IASetVertexBuffers(0, 1, &vbuffer->vbv);
		command_list()->IASetIndexBuffer(&ibuffer->ibv);
		command_list()->DrawIndexedInstanced(n_indices, 1, 0, 0, 0);

		command_list()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(rt->resource.Get(), (D3D12_RESOURCE_STATES)rt->resource_state, D3D12_RESOURCE_STATE_PRESENT));
		rt->resource_state = D3D12_RESOURCE_STATE_PRESENT;
		command_list()->Close();

		ID3D12CommandList* lists[] = { command_list() };
		_render_device->command_queue()->ExecuteCommandLists(1, lists);

		_render_device->command_list_factory()->executed(command_list);

		_render_device->present();
		_render_device->wait_for_fence();

		return S_OK;
	}



};


/////////////////////////////////////////


}