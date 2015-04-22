#include "render_context.h"
#include "render_resource_manager.h"
#include "compiled_drawcall.h"

#define DIRTY(mask, state) (mask & (1<<state))

namespace ue
{

D3D11RenderContext::D3D11RenderContext(ID3D11DeviceContext* context)
: _context(context)
{
}

D3D11RenderContext::~D3D11RenderContext()
{
}

void D3D11RenderContext::dispatch()
{
	RenderCommand* cmd;
	while(command_stream.get(&cmd))
	{
		switch(cmd->type)
		{
			case RenderCommandType::DrawCall:
			{
				drawcall((DrawCallCommand*)cmd);
			} break;

		}
	}
}

void D3D11RenderContext::collect_resources(unsigned resource_count, const RenderResource* resources, unsigned* states)
{
	D3D11ResourceManager* resource_manager = _resource_manager;
	for(unsigned i = 0; i < resource_count; ++i)
	{
		const RenderResource& resource = resources[i];
		switch(resource.type)
		{
			case ResourceType::DepthStencilState:
			{

				//resource_manager->get_depth_stencil_state(resource.handle, )
			}
		}
	}
}

void D3D11RenderContext::drawcall(DrawCallCommand* cmd)
{
	unsigned dirty_mask;

	//collect_resources(cmd, )

	D3D11CompiledDrawCall* compiled = _resource_manager->get_cached_drawcall(cmd->drawcall_hash);

	if(compiled == NULL)
	{
		
	}

	if(DIRTY(dirty_mask, DepthStencilState))
		_context->OMSetDepthStencilState(compiled->depth_stencil_state, *compiled->stencil_ref);
	if(DIRTY(dirty_mask, BlendState))
		_context->OMSetBlendState(compiled->blend_state, compiled->blend_factor, *compiled->sample_mask);
	if(DIRTY(dirty_mask, RasterizerState))		
		_context->RSSetState(compiled->rasterizer_state);
	//if(DIRTY(dirty_mask, ViewPort))
	//	_context->RSSetViewports(0, NULL);
	//if(DIRTY(dirty_mask, Scissors))
	//	_context->RSSetScissorRects(0, NULL);

	if(DIRTY(dirty_mask, VSShader))
		_context->VSSetShader(compiled->vertex_shader, NULL, 0);
	if(DIRTY(dirty_mask, VSSamplers))
		_context->VSSetSamplers(*compiled->vs_samplers_start_slot, *compiled->vs_samplers_count, compiled->vs_samplers);
	if(DIRTY(dirty_mask, VSResources))
		_context->VSSetShaderResources(*compiled->vs_resource_start_slot, *compiled->vs_resource_count, compiled->vs_shader_resource_views);
	if(DIRTY(dirty_mask, VSCBuffers))
		_context->VSSetConstantBuffers(*compiled->vs_cb_start_slot, *compiled->vs_cb_count, compiled->vs_constant_buffers);

	if (DIRTY(dirty_mask, PSShader))
		_context->PSSetShader(compiled->pixel_shader, NULL, 0);
	if (DIRTY(dirty_mask, PSSamplers))
		_context->PSSetSamplers(*compiled->ps_samplers_start_slot, *compiled->ps_samplers_count, compiled->ps_samplers);
	if (DIRTY(dirty_mask, PSResources))
		_context->PSSetShaderResources(*compiled->ps_resource_start_slot, *compiled->ps_resource_count, compiled->ps_shader_resource_views);
	if (DIRTY(dirty_mask, PSCBuffers))
		_context->PSSetConstantBuffers(*compiled->ps_cb_start_slot, *compiled->ps_cb_count, compiled->ps_constant_buffers);


	if (DIRTY(dirty_mask, GSShader))
		_context->GSSetShader(NULL, NULL, 0);
	if (DIRTY(dirty_mask, GSSamplers))
		_context->GSSetSamplers(0, 0, NULL);
	if (DIRTY(dirty_mask, GSResources))
		_context->GSSetShaderResources(0, 0, NULL);
	if (DIRTY(dirty_mask, GSCBuffers))
		_context->GSSetConstantBuffers(0, 0, NULL);

	if (DIRTY(dirty_mask, HSShader))
		_context->HSSetShader(NULL, NULL, 0);
	if (DIRTY(dirty_mask, HSSamplers))
		_context->HSSetSamplers(0, 0, NULL);
	if (DIRTY(dirty_mask, HSResources))
		_context->HSSetShaderResources(0, 0, NULL);
	if (DIRTY(dirty_mask, HSCBuffers))
		_context->HSSetConstantBuffers(0, 0, NULL);

	if (DIRTY(dirty_mask, DSShader))
		_context->DSSetShader(NULL, NULL, 0);
	if (DIRTY(dirty_mask, DSSamplers))
		_context->DSSetSamplers(0, 0, NULL);
	if (DIRTY(dirty_mask, DSResources))
		_context->DSSetShaderResources(0, 0, NULL);
	if (DIRTY(dirty_mask, DSCBuffers))
		_context->DSSetConstantBuffers(0, 0, NULL);
	
	if (DIRTY(dirty_mask, InputLayout))
		_context->IASetInputLayout(compiled->input_layout);
	if (DIRTY(dirty_mask, IndexBuffer))
		_context->IASetIndexBuffer(compiled->index_buffer, *compiled->index_format, *compiled->index_offset);
	if (DIRTY(dirty_mask, VertexBuffers))
		_context->IASetVertexBuffers(*compiled->vb_start_slot, *compiled->vb_count, compiled->vb_buffers, compiled->vb_strides, compiled->vb_offsets);

	if (DIRTY(dirty_mask, PrimitiveTopology))
		_context->IASetPrimitiveTopology(compiled->primitive_topology);

	if(compiled->draw_call_type == DrawCallType::Indexed)
		_context->DrawIndexed(compiled->index_count, compiled->start_index_location, compiled->base_vertex_location);
	else if(compiled->draw_call_type == DrawCallType::Instanced)
		_context->DrawIndexedInstanced(compiled->index_count, compiled->instance_count, compiled->start_index_location, compiled->base_vertex_location, compiled->instance_count);
}

void D3D11RenderContext::compute()
{
	_context->CSSetShader(NULL, NULL, 0);
	_context->CSSetSamplers(0, 0, NULL);
	_context->CSSetShaderResources(0, 0, NULL);
	_context->CSSetConstantBuffers(0, 0, NULL);

	_context->Dispatch(0, 0, 0);
}


StateMask D3D11RenderContext::generate_drawcall_dirty_mask(const unsigned* drawcall_states)
{
	StateMask dirty_mask = 0;
	for(unsigned i = 0; i < DrawCallStatesCount; ++i)
	{
		if(drawcall_states[i] != _drawcall_states[i])
		{
			dirty_mask |= ((StateMask)1<<i);
			_drawcall_states[i] = drawcall_states[i];
		}
	}
	return dirty_mask;
}

}