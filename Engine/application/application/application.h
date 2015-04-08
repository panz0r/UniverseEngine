#pragma once

#include <core/thread/thread.h>

namespace em
{

class Application
{
public:
	Application();
	~Application();

	void initialize();
	int run();

private:

	FiberPool* _fiber_pool;
	WorkerThread* _worker_threads;

};


}