#pragma once



namespace ue
{

class D3D12RenderDevice;

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
};


}