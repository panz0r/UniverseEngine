#include "render_context.h"
#include <renderer_d3d11/resource/render_resource_manager.h>



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
	//RenderCommand* cmd;
	//while(command_stream.get(&cmd))
	//{
	//	switch(cmd->type)
	//	{
	//		case RenderCommandType::DrawCall:
	//		{
	//			drawcall((DrawCallCommand*)cmd);
	//		} break;

	//	}
	//}
}


void D3D11RenderContext::drawcall(DrawCallCommand* cmd)
{
	unsigned dirty_mask;


	D3D11RenderAtom* atom; // = _resource_manager->get_render_atom(cmd->atom_handle);

	if(atom == NULL)
	{
		
	}

	if (DIRTY(dirty_mask, DepthStencilState))
		_context->OMSetDepthStencilState((ID3D11DepthStencilState*)atom->ptr(D3D11RenderAtom::DepthStencilState), atom->uint(D3D11RenderAtom::StencilRef));
	if(DIRTY(dirty_mask, BlendState))
		_context->OMSetBlendState((ID3D11BlendState*)atom->ptr(D3D11RenderAtom::BlendState), (float*)atom->ptr(D3D11RenderAtom::BlendFactor), atom->uint(D3D11RenderAtom::SampleMask));
	if(DIRTY(dirty_mask, RasterizerState))
		_context->RSSetState((ID3D11RasterizerState*)atom->ptr(D3D11RenderAtom::RasterizerState));

	// Should be set with a different render command for this renderer, is probably baked into the draw state for d3d12
	//if(DIRTY(dirty_mask, ViewPort))
	//	_context->RSSetViewports(0, NULL);
	//if(DIRTY(dirty_mask, Scissors))
	//	_context->RSSetScissorRects(0, NULL);

	if(DIRTY(dirty_mask, VSShader))
		_context->VSSetShader((ID3D11VertexShader*)atom->ptr(D3D11RenderAtom::VertexShader), NULL, 0);
	if(DIRTY(dirty_mask, VSSamplers))
		_context->VSSetSamplers(atom->uint(D3D11RenderAtom::VSSamplerStart), atom->uint(D3D11RenderAtom::VSSamplerCount), (ID3D11SamplerState**)atom->ptrptr(D3D11RenderAtom::VSSamplers));
	if(DIRTY(dirty_mask, VSResources))
		_context->VSSetShaderResources(atom->uint(D3D11RenderAtom::VSResourceStart), atom->uint(D3D11RenderAtom::VSResourceCount), (ID3D11ShaderResourceView**)atom->ptrptr(D3D11RenderAtom::VSResources));
	if (DIRTY(dirty_mask, VSCBuffers))
		_context->VSSetConstantBuffers(atom->uint(D3D11RenderAtom::VSCBStart), atom->uint(D3D11RenderAtom::VSCBCount), (ID3D11Buffer**)atom->ptrptr(D3D11RenderAtom::VSConstantBuffers));

	if (DIRTY(dirty_mask, PSShader))
		_context->PSSetShader((ID3D11PixelShader*)atom->ptr(D3D11RenderAtom::PixelShader), NULL, 0);
	if (DIRTY(dirty_mask, PSSamplers))
		_context->PSSetSamplers(atom->uint(D3D11RenderAtom::PSSamplerStart), atom->uint(D3D11RenderAtom::PSSamplerCount), (ID3D11SamplerState**)atom->ptrptr(D3D11RenderAtom::PSSamplers));
	if (DIRTY(dirty_mask, PSResources))
		_context->PSSetShaderResources(atom->uint(D3D11RenderAtom::PSResourceStart), atom->uint(D3D11RenderAtom::PSResourceCount), (ID3D11ShaderResourceView**)atom->ptrptr(D3D11RenderAtom::PSResources));
	if (DIRTY(dirty_mask, PSCBuffers))
		_context->PSSetConstantBuffers(atom->uint(D3D11RenderAtom::PSCBStart), atom->uint(D3D11RenderAtom::PSCBCount), (ID3D11Buffer**)atom->ptrptr(D3D11RenderAtom::PSConstantBuffers));

	// Extended drawcall parameters, rare and should be handled in a different path
	//if (DIRTY(dirty_mask, GSShader))
	//	_context->GSSetShader(NULL, NULL, 0);
	//if (DIRTY(dirty_mask, GSSamplers))
	//	_context->GSSetSamplers(0, 0, NULL);
	//if (DIRTY(dirty_mask, GSResources))
	//	_context->GSSetShaderResources(0, 0, NULL);
	//if (DIRTY(dirty_mask, GSCBuffers))
	//	_context->GSSetConstantBuffers(0, 0, NULL);

	//if (DIRTY(dirty_mask, HSShader))
	//	_context->HSSetShader(NULL, NULL, 0);
	//if (DIRTY(dirty_mask, HSSamplers))
	//	_context->HSSetSamplers(0, 0, NULL);
	//if (DIRTY(dirty_mask, HSResources))
	//	_context->HSSetShaderResources(0, 0, NULL);
	//if (DIRTY(dirty_mask, HSCBuffers))
	//	_context->HSSetConstantBuffers(0, 0, NULL);

	//if (DIRTY(dirty_mask, DSShader))
	//	_context->DSSetShader(NULL, NULL, 0);
	//if (DIRTY(dirty_mask, DSSamplers))
	//	_context->DSSetSamplers(0, 0, NULL);
	//if (DIRTY(dirty_mask, DSResources))
	//	_context->DSSetShaderResources(0, 0, NULL);
	//if (DIRTY(dirty_mask, DSCBuffers))
	//	_context->DSSetConstantBuffers(0, 0, NULL);
	
	if (DIRTY(dirty_mask, InputLayout))
		_context->IASetInputLayout((ID3D11InputLayout*)atom->ptr(D3D11RenderAtom::InputLayout));
	if (DIRTY(dirty_mask, IndexBuffer))
		_context->IASetIndexBuffer((ID3D11Buffer*)atom->ptr(D3D11RenderAtom::IndexBuffer), (DXGI_FORMAT)atom->uint(D3D11RenderAtom::IndexFormat), atom->uint(D3D11RenderAtom::IndexOffset));
	if (DIRTY(dirty_mask, VertexBuffers))
		_context->IASetVertexBuffers(atom->uint(D3D11RenderAtom::VBStart), atom->uint(D3D11RenderAtom::VBCount), (ID3D11Buffer**)atom->ptrptr(D3D11RenderAtom::VBuffers), (unsigned*)atom->ptr(D3D11RenderAtom::VBStrides), (unsigned*)atom->ptr(D3D11RenderAtom::VBOffsets));

	if (DIRTY(dirty_mask, PrimitiveTopology))
		_context->IASetPrimitiveTopology(atom->primitive_topology);

	if(atom->draw_call_type == DrawCallType::Indexed)
		_context->DrawIndexed(atom->index_count, atom->start_index_location, atom->base_vertex_location);
	else if(atom->draw_call_type == DrawCallType::Instanced)
		_context->DrawIndexedInstanced(atom->index_count, atom->instance_count, atom->start_index_location, atom->base_vertex_location, atom->instance_count);
}

void D3D11RenderContext::compute()
{
	_context->CSSetShader(NULL, NULL, 0);
	_context->CSSetSamplers(0, 0, NULL);
	_context->CSSetShaderResources(0, 0, NULL);
	_context->CSSetConstantBuffers(0, 0, NULL);

	_context->Dispatch(0, 0, 0);
}


StateMask D3D11RenderContext::generate_drawcall_dirty_mask(const unsigned* drawcall_states, unsigned count)
{
	// TODO, measure performance accessing member states vs copying them to stack, keep in mind we need to copy them back as well
	StateMask dirty_mask = 0;
	for(unsigned i = 0; i < count; ++i)
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