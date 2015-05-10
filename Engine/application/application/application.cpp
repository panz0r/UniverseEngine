#include "application.h"

#include <core/thread/thread.h>


#include <renderer_d3d11/renderer/render_device.h>
#include <renderer_d3d11/renderer/render_context.h>

#include <raytracer/raytracer.h>

namespace ue
{

Application::Application(void* window_handle)
: _window_handle(window_handle)
{}

Application::~Application()
{}

void Application::initialize()
{
	_fiber_system = new FiberSystem(8, 1000);
	
	//D3D11RenderDevice* device = new D3D11RenderDevice((HWND)_window_handle, 1024, 768);
	//D3D11RenderContext* context = new D3D11RenderContext(device->context());

	//unsigned states = rand();
	//context->drawcall(states);
}


int Application::run()
{
	unsigned width = 256;
	unsigned height = 256;
	char* pixels = new char[width*height*3];

	RaytraceParams params;
	params.width = width;
	params.height = height;
	Job raytrace_job = Job(start_job, &params);

	//JobDeclaration job = JobDeclaration(&test_job, NULL);
	Counter* counter = NULL;
	Counter counter = schedule_jobs(&raytrace_job, 1, &counter);

	while(!counter->is_zero())
	{
		Sleep(1);
	}

return 1;
}

}