#include "application.h"

#include <core/platform/platform.h>
#include <core/thread/thread.h>
#include <core/thread/fiber_system.h>

#include <renderer_d3d12\renderer\render_device.h>


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

	D3D12RenderDeviceDesc desc;
	desc.window_handle = (HWND)_window_handle;
	desc.debug = true;
	desc.back_buffer_count = 2;
	desc.width = 640;
	desc.height = 480;
	desc.fullscreen = false;
	
	_render_device = new D3D12RenderDevice(desc);
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