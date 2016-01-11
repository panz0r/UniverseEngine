
#include "test_base.h"

#include <renderer_d3d12/renderer/d3d12_render_device.h>
#include <renderer_d3d12/memory/d3d12_memory.h>
#include <renderer_d3d12/resource/d3d12_shader_factory.h>
#include <renderer_d3d12/resource/d3d12_resource_manager.h>
#include <renderer_d3d12/renderer/d3d12_command_list_factory.h>

namespace ue
{


/////////////////////////////////////////

class FullScreenTriangleTest : public UnitTestBase
{
public:
	D3D12RenderDevice* _render_device;
	D3D12ShaderFactory* _shader_factory;

	RenderHandle back_buffers[2];
	D3D12_VIEWPORT viewport;
	D3D12_RECT scissor_rect;
	RenderSetHandle pipeline_state_handle;
	RenderSetHandle root_signature_handle;

public:

	FullScreenTriangleTest(unsigned width, unsigned height, const char* name)
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
	#define RS1 "CBV(b0) "
		
	struct VS_OUT
	{
		float4 position : SV_Position;
		float2 texcoord : TEXCOORD0;
	};
		
	VS_OUT vs_main(uint id : SV_VertexID)
	{
		VS_OUT vs_out;
		float2 texcoord;
		texcoord.x = (id == 2) ? 2.0 : 0.0;
		texcoord.y = (id == 1) ? 2.0 : 0.0;
		vs_out.position = float4(texcoord * float2(2.0, -2.0) + float2(-1.0, 1.0), 1.0, 1.0);
		vs_out.texcoord = texcoord;
		return vs_out;
	}

							
	float4 ps_main(VS_OUT ps_in) : SV_Target
	{
		return float4(ps_in.texcoord, 1.0, 1.0);
	}

				)";

		auto vertex_shader = _shader_factory->compile_shader_from_source(shader, "vs_main", "vs_5_0", 0);
		auto pixel_shader = _shader_factory->compile_shader_from_source(shader, "ps_main", "ps_5_0", 0);
		auto root_signature = _shader_factory->compile_root_signature_from_source(shader, "RS1", 0);
		auto root_signature_handle = _render_device->resource_manager()->create_root_signature(root_signature);

		back_buffers[0] = _render_device->resource_manager()->create_render_target_from_resource(_render_device->get_back_buffer(0), D3D12_RESOURCE_STATE_PRESENT);
		back_buffers[1] = _render_device->resource_manager()->create_render_target_from_resource(_render_device->get_back_buffer(1), D3D12_RESOURCE_STATE_PRESENT);

		D3D12GraphicsPipelineStateDesc desc = {};
		desc.root_signature = *_render_device->resource_manager()->root_signature(root_signature_handle);
		desc.shaders[VertexShader] = vertex_shader;
		desc.shaders[PixelShader] = pixel_shader;
		desc.primitive_topology_type = (unsigned)D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		desc.render_target_count = 1;
		desc.render_target_formats[0] = _render_device->resource_manager()->render_target(back_buffers[0])->format;
		desc.depth_stencil_format = (unsigned)DXGI_FORMAT_UNKNOWN;
		pipeline_state_handle = _render_device->resource_manager()->create_graphics_pipeline_state(desc);


	}
	
	virtual ~FullScreenTriangleTest()
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
		command_list()->DrawInstanced(3, 1, 0, 0);

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


}