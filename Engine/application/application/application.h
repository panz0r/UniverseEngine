#pragma once


namespace em
{

class FiberPool;
struct WorkerThread;
class Scheduler;

class Application
{
public:
	Application();
	~Application();

	void initialize();
	int run();

private:

	FiberPool* _fiber_pool;
	Scheduler* _scheduler;
	WorkerThread* _worker_threads;

};


}