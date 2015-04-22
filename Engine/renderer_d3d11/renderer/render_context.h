#pragma once

#include "render_command.h"

#include <d3d11.h>


namespace ue
{

struct RenderResource;
class D3D11ResourceManager;


typedef unsigned StateMask;

class D3D11RenderContext
{
public:
	D3D11RenderContext(ID3D11DeviceContext* context);
	~D3D11RenderContext();
	
	void dispatch();

	void drawcall(DrawCallCommand* cmd);
	void compute();

private:
	void collect_resources(unsigned resource_count, const RenderResource* resource_handles);
	StateMask generate_drawcall_dirty_mask(const unsigned* drawcall_states);

	unsigned _drawcall_states[DrawCallStatesCount];
	ID3D11DeviceContext* _context;
	D3D11ResourceManager* _resource_manager;
};


}