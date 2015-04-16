#pragma once


namespace ue
{

class Application
{
public:
	Application(void* window_handle);
	~Application();

	void initialize();
	int run();

private:

	void* _window_handle;
};


}