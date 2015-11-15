#include "command_list_factory.h"

namespace ue
{

__THREAD_LOCAL GraphicsCommandLists *CommandListFactory::_graphics_command_lists = nullptr;
__THREAD_LOCAL CommandListFreeList *CommandListFactory::_graphics_command_lists_free = nullptr;

CommandListFactory::CommandListFactory(D3D12RenderDevice &render_device)
: _render_device(render_device)
{
}

CommandListFactory::~CommandListFactory()
{

}

void CommandListFactory::thread_initialize()
{
	_graphics_command_lists = new GraphicsCommandLists();
	_graphics_command_lists_free = new CommandListFreeList();
}

}