#include "fiber_system.h"
#include "fiber.h"
#include "thread.h"

//#define FIBER_DEBUG
#if defined(FIBER_DEBUG)
#define DEBUG_FIBER_SWITCH(tag, fiber) \
	char temp_buffer[128]; \
	sprintf(temp_buffer, "(%d): (%s), From: %08x, To: %08x\n", GetCurrentThreadId(), tag, GetCurrentFiber(), fiber); \
	OutputDebugString(temp_buffer); \
	assert(fiber != NULL);

#define DEBUG_FIBER_RELASE(fiber) \
	char temp_buffer[128]; \
	sprintf(temp_buffer, "(%d): (release): %08x\n", GetCurrentThreadId(), fiber); \
	OutputDebugString(temp_buffer);
#else
#define DEBUG_FIBER_SWITCH(tag, fiber)
#define DEBUG_FIBER_RELASE(fiber)
#endif

namespace ue
{


///////////////////////////////////////////////////////////////////////////////////

__declspec(thread)	void *tls_current_fiber;
__declspec(thread)	void *tls_next_fiber;
__declspec(thread)	AtomicCounter *tls_counter;
__declspec(thread) int tls_counter_value;
__declspec(thread) unsigned tls_thread_id;


FiberSystem::FiberSystem()
	: _waiting_jobs(0)
{
	InitializeCriticalSection(&_waiting_job_lock);
	_quit.store(false);
}


FiberSystem::~FiberSystem()
{
}

void FiberSystem::initialize(FiberSystemParams *fiber_system_params, unsigned thread_count, unsigned fiber_pool_count)
{
	_thread_count = thread_count;
	const unsigned fiber_stack_size = 128 * 1024;
	for (unsigned i = 0; i < fiber_pool_count; ++i) {
		_fiber_pool.enqueue( create_fiber(fiber_stack_size, FiberFunction, fiber_system_params) );
	}
	_threads = new void*[thread_count];
	_switching_fibers = new void*[thread_count];
	_counter_waiting_fibers = new void*[thread_count];

	const unsigned utility_fiber_stack_size = 32 * 1024;
	for (unsigned i = 0; i < thread_count; ++i) {
		_switching_fibers[i] = create_fiber(utility_fiber_stack_size, SwitchFiberFunction, fiber_system_params);
		_counter_waiting_fibers[i] = create_fiber(utility_fiber_stack_size, WaitOnCounterFiberFunction, fiber_system_params);
	}


	_threads[0] = get_current_thread();
	set_thread_affinity_mask(_threads[0], 1);
	tls_thread_id = 0;

	for (unsigned i = 1; i < thread_count; ++i)
	{
		ThreadParams *thread_params = new ThreadParams();
		thread_params->thread_id = i;
		thread_params->fiber_system_params = fiber_system_params;

		_threads[i] = create_worker_thread(ThreadFunction, (1 << i), thread_params);
	}
	
	convert_thread_to_fiber(nullptr);

}

void FiberSystem::wait_for_counter(Counter& counter, int value)
{
	if (counter->load() == value) {
		return;
	}

	tls_current_fiber = get_current_fiber();
	tls_counter_value = value;
	tls_counter = counter.get();
	_fiber_pool.wait_dequeue(tls_next_fiber);
	switch_to_fiber(_counter_waiting_fibers[tls_thread_id]);
}

Counter FiberSystem::schedule_jobs(Job *jobs, unsigned count, unsigned priortiy)
{
	Counter counter = Counter(new AtomicCounter());
	counter->store(count);
	for (unsigned i = 0; i < count; ++i) {
		JobBundle bundle = { jobs[i], counter };
		_jobs[priortiy].enqueue(bundle);
	}
	return counter;
}

void FiberSystem::quit() {
	_quit.store(true);

	convert_fiber_to_thread();

	std::vector<HANDLE> workerThreads;
	for (unsigned i = 0; i < _thread_count; ++i) {
		if (_threads[i] != GetCurrentThread()) {
			workerThreads.push_back(_threads[i]);
		}
	}

	DWORD result = WaitForMultipleObjects((DWORD)workerThreads.size(), &workerThreads[0], true, 100);

	for (auto &workerThread : workerThreads) {
		CloseHandle(workerThread);
	}
}

unsigned long __stdcall FiberSystem::ThreadFunction(void *params)
{
	ThreadParams* thread_params = (ThreadParams*)params;
	tls_thread_id = thread_params->thread_id;
	FiberSystemParams* fiber_system_params = thread_params->fiber_system_params;
	delete thread_params;

	convert_thread_to_fiber(nullptr);

	FiberFunction(fiber_system_params);

	convert_fiber_to_thread();
	
	return 1;	
}

void __stdcall FiberSystem::FiberFunction(void *params)
{
	FiberSystemParams *fiber_system_params = (FiberSystemParams*)params;
	FiberSystem &fiber_system = fiber_system_params->_fiber_system;

	while (fiber_system._quit.load() == false)
	{
		WaitingJob waiting_job;
		bool have_waiting_job = false;
		EnterCriticalSection(&fiber_system._waiting_job_lock);

		//if (fiber_system._spin_lock.try_lock()) {
		//	for (unsigned i = 0; i < fiber_system._waiting_jobs; ++i) {
		//		WaitingJob &waiting_job = fiber_system._wait_list[i];
		//		if (waiting_job._counter->load() == waiting_job._value) {
		//			waiting_fiber = waiting_job._fiber;
		//			std::swap(fiber_system._wait_list[i], fiber_system._wait_list[fiber_system._waiting_jobs-1]);
		//			fiber_system._waiting_jobs--;
		//			break;
		//		}
		//	}
		//	fiber_system._spin_lock.unlock();
		//}
		
		auto iter = fiber_system._wait_list.begin();
		for (; iter != fiber_system._wait_list.end(); ++iter) {
			if (iter->_counter->load() == iter->_value) {
				have_waiting_job = true;
				break;
			}
		}

		if (have_waiting_job) {
			waiting_job = *iter;
			fiber_system._wait_list.erase(iter);
		}

		LeaveCriticalSection(&fiber_system._waiting_job_lock);
	
		if (have_waiting_job) {
			tls_current_fiber = get_current_fiber();
			tls_next_fiber = waiting_job._fiber;
			switch_to_fiber(fiber_system._switching_fibers[tls_thread_id]);
		}

		JobBundle job_bundle;
		bool have_job = false;
		for (unsigned i = 0; i < PRIORITY_COUNT; ++i) {
			if(fiber_system._jobs[i].try_dequeue(job_bundle)) {
				have_job = true;
				break;
			}
		}

		if (have_job) {
			job_bundle.job.Function(&fiber_system, job_bundle.job._params);
			job_bundle.counter->fetch_sub(1);
		}
		else {
			//SwitchToThread();	// yield for os threads
		}


	}

}

void __stdcall FiberSystem::SwitchFiberFunction(void *params)
{
	FiberSystemParams *fiber_system_params = (FiberSystemParams*)params;
	FiberSystem &fiber_system = fiber_system_params->_fiber_system;

	while (true) {
		fiber_system._fiber_pool.enqueue(tls_current_fiber);
		switch_to_fiber(tls_next_fiber);
	}

}

void __stdcall FiberSystem::WaitOnCounterFiberFunction(void *params)
{
	FiberSystemParams *fiber_system_params = (FiberSystemParams*)params;
	FiberSystem &fiber_system = fiber_system_params->_fiber_system;

	while (true) {
		EnterCriticalSection(&fiber_system._waiting_job_lock);
		fiber_system._wait_list.emplace_back(WaitingJob(tls_current_fiber, tls_counter, tls_counter_value));
		
		LeaveCriticalSection(&fiber_system._waiting_job_lock);
		
		switch_to_fiber(tls_next_fiber);
	}
}





}