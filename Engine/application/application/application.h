#pragma once

#include <renderer_d3d12/com/com_ptr.h>
#include <d3d12.h>

namespace ue
{

class D3D12RenderDevice;
class CommandListFactory;
class ResourceManager;
class DescriptorHeapFactory;

class Application
{
public:
	Application(void* window_handle);
	~Application();

	void initialize();
	int run();

private:

	void* _window_handle;
	D3D12RenderDevice *_render_device;
	CommandListFactory *_command_list_factory;
	ResourceManager *_resource_manager;
	DescriptorHeapFactory *_descriptor_heap_factory;


};


}