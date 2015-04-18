#include "render_context.h"

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


void D3D11RenderContext::drawcall(statemask_t dirty_mask)
{
	if(DIRTY(dirty_mask, DepthStencilState))
		_context->OMSetDepthStencilState(NULL, 0);
	if(DIRTY(dirty_mask, BlendState))
		_context->OMSetBlendState(NULL, NULL, 0);
	if(DIRTY(dirty_mask, RasterizerState))		
		_context->RSSetState(NULL);
	if(DIRTY(dirty_mask, ViewPort))
		_context->RSSetViewports(0, NULL);
	if(DIRTY(dirty_mask, Scissors))
		_context->RSSetScissorRects(0, NULL);

	if(DIRTY(dirty_mask, VSShader))
		_context->VSSetShader(NULL, NULL, 0);
	if(DIRTY(dirty_mask, VSSamplers))
		_context->VSSetSamplers(0, 0, NULL);
	if(DIRTY(dirty_mask, VSResources))
		_context->VSSetShaderResources(0, 0, NULL);
	if(DIRTY(dirty_mask, VSCBuffers))
		_context->VSSetConstantBuffers(0, 0, NULL);

	if (DIRTY(dirty_mask, PSShader))
		_context->PSSetShader(NULL, NULL, 0);
	if (DIRTY(dirty_mask, PSSamplers))
		_context->PSSetSamplers(0, 0, NULL);
	if (DIRTY(dirty_mask, PSResources))
		_context->PSSetShaderResources(0, 0, NULL);
	if (DIRTY(dirty_mask, PSCBuffers))
		_context->PSSetConstantBuffers(0, 0, NULL);

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
		_context->IASetInputLayout(NULL);
	if (DIRTY(dirty_mask, PrimitiveTopology))
		_context->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)0);
	if (DIRTY(dirty_mask, IndexBuffer))
		_context->IASetIndexBuffer(NULL, (DXGI_FORMAT)0, 0);
	if (DIRTY(dirty_mask, VertexBuffers))
		_context->IASetVertexBuffers(0, 0, NULL, NULL, NULL);

	_context->DrawIndexed(0, 0, 0);
	_context->DrawIndexedInstanced(0, 0, 0, 0, 0);
}

void D3D11RenderContext::compute()
{
	_context->CSSetShader(NULL, NULL, 0);
	_context->CSSetSamplers(0, 0, NULL);
	_context->CSSetShaderResources(0, 0, NULL);
	_context->CSSetConstantBuffers(0, 0, NULL);

	_context->Dispatch(0, 0, 0);
}


statemask_t D3D11RenderContext::compute_drawcall_dirty_mask(const unsigned* drawcall_states)
{
	statemask_t dirty_mask = 0;
	for(unsigned i = 0; i < DrawCallStatesCount; ++i)
	{
		if(drawcall_states[i] != _drawcall_states[i])
		{
			dirty_mask |= ((statemask_t)1<<i);
			_drawcall_states[i] = drawcall_states[i];
		}
	}
	return dirty_mask;
}

}