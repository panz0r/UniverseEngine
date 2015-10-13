#include "application.h"

#include <core/platform/platform.h>
#include <core/thread/thread.h>
#include <core/thread/fiber_system.h>

#include <renderer_d3d12\renderer\render_device.h>
#include <renderer_d3d12\renderer\resource_manager.h>

#include <renderer_d3d12\memory\buddy_allocator.h>

namespace ue
{

Application::Application(void* window_handle)
: _window_handle(window_handle)
{}

Application::~Application()
{
	delete _render_device;
}

void Application::initialize()
{
	size_t buffer_size = 1024*1024*128;
	void *buffer = malloc(buffer_size);
	BuddyAllocatorExt apa(buffer, buffer_size, 64*1024);

	void *p1 = apa.allocate(22);
	void *p2 = apa.allocate(13);
	void *p3 = apa.allocate(64);
	void *p4 = apa.allocate(128);
	void *p5 = apa.allocate(128);
	void *p6 = apa.allocate(64);
	void *p7 = apa.allocate(64);
	apa.deallocate(p1);
	apa.deallocate(p4);
	apa.deallocate(p6);
	apa.deallocate(p3);



	D3D12RenderDeviceDesc desc;
	desc.window_handle = (HWND)_window_handle;
	desc.debug = true;
	desc.back_buffer_count = 2;
	desc.width = 640;
	desc.height = 480;
	desc.fullscreen = false;
	
	_render_device = new D3D12RenderDevice(desc);

	auto resource_manager = new ResourceManager(*_render_device);

	TextureDesc tex_desc;
	tex_desc.width = 256;
	tex_desc.height = 256;
	tex_desc.depth = 1;
	tex_desc.dimension = 3;
	tex_desc.format = 0;
	tex_desc.mip_levels = 1;
	tex_desc.dynamic = false;

	auto texture = resource_manager->create_texture(tex_desc, nullptr);
	auto texture2 = resource_manager->create_texture(tex_desc, nullptr);
	resource_manager->destroy_texture(texture);

	//D3D11RenderDevice* device = new D3D11RenderDevice((HWND)_window_handle, 1024, 768);
	//D3D11RenderContext* context = new D3D11RenderContext(device->context());

	//unsigned states = rand();
	//context->drawcall(states);
}


int Application::run()
{
	FiberSystemParams* fiber_system_params = new FiberSystemParams();
	FiberSystem &fiber_system = fiber_system_params->_fiber_system;

	fiber_system.initialize(fiber_system_params, platform::hardware_thread_count(), 1024);
	//fiber_system.wait_for_counter(counter, 0);

	fiber_system.quit();


return 1;
}

}