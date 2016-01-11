#pragma once

#include <renderer_d3d12/com/com_ptr.h>
#include <d3d12.h>

namespace ue
{

class D3D12RenderDevice;
class D3D12CommandListFactory;
class D3D12ResourceManager;
class OnlineDescriptorHeapFactory;

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
	D3D12CommandListFactory *_command_list_factory;
	D3D12ResourceManager *_resource_manager;
	OnlineDescriptorHeapFactory *_descriptor_heap_factory;


};


}