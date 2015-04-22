#pragma once

namespace ue
{

enum class RenderCommandType : unsigned
{
	DrawCall,
	Clear,
};

struct RenderCommand
{
	RenderCommandType type;
};


struct DrawCallCommand
{
	RenderCommand base;

	unsigned drawcall_hash;

};


}