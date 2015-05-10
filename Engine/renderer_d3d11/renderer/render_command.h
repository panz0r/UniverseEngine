#pragma once

#include <core/handle/handle.h>

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

	Handle atom_handle;

};


}