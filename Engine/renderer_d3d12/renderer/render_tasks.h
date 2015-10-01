#pragma once

#include <core/platform/platform.h>
#include <d3d12.h>

namespace ue
{

class D3D12RenderDevice;
class ResourceManager;

template <class T>
T* next_render_atom(T* render_atom) {
	return reinterpret_cast<T*>(reinterpret_cast<char*>(render_atom) + render_atom->size);
}


class DrawInstancedTask
{
public:
	static void execute(void *user_data);

};

}