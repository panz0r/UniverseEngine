#include "application.h"

#include <core/thread/thread.h>


#include <renderer_d3d11/renderer/render_device.h>
#include <renderer_d3d11/renderer/render_context.h>

#include <raytracer/raytracer.h>

#include <core/thread/fiber_system.h>

namespace ue
{

Application::Application(void* window_handle)
: _window_handle(window_handle)
{}

Application::~Application()
{}

void Application::initialize()
{
	
	//D3D11RenderDevice* device = new D3D11RenderDevice((HWND)_window_handle, 1024, 768);
	//D3D11RenderContext* context = new D3D11RenderContext(device->context());

	//unsigned states = rand();
	//context->drawcall(states);
}


int Application::run()
{
	FiberSystemParams* fiber_system_params = new FiberSystemParams();
	FiberSystem &fiber_system = fiber_system_params->_fiber_system;

	fiber_system.initialize(fiber_system_params, 8, 1024);

	unsigned width = 1024;
	unsigned height = 1024;
	char* pixels = new char[width*height*3];

	RaytraceParams params;
	params.width = width;
	params.height = height;
	Job raytrace_job = Job(start_job, &params);

	//JobDeclaration job = JobDeclaration(&test_job, NULL);

	Counter counter = fiber_system.schedule_jobs(&raytrace_job, 1);
	fiber_system.wait_for_counter(counter, 0);

	fiber_system.quit();


return 1;
}

}