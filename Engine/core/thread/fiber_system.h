#pragma once


#include <memory>
#include <vector>
#include <list>
#include <concurrentqueue/concurrentqueue.h>
#include <concurrentqueue/blockingconcurrentqueue.h>
#include "atomic_spin_lock.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace ue
{

enum { PRIORITY_HIGH, PRIORITY_NORMAL, PRIORITY_LOW, PRIORITY_COUNT };

struct JobDeclaration;


typedef void (*JobFunction)(class FiberSystem *fiber_system, void *params);
#define DECLARE_JOB(name)	void name(FiberSystem *fiber_system, void *params)

struct Job
{
	Job() {}
	Job(JobFunction function, void *params)
		: Function(function), _params(params)
	{}

	JobFunction Function;
	void *_params;
};

struct FiberSystemParams;

struct ThreadParams
{
	FiberSystemParams *fiber_system_params;
	unsigned thread_id;
};

typedef std::atomic_int32_t AtomicCounter;
typedef std::shared_ptr<AtomicCounter> Counter;

class FiberSystem
{
public:
	FiberSystem();
	~FiberSystem();

	void initialize(FiberSystemParams *fiber_system_params, unsigned thread_count, unsigned fiber_pool_count);
		
	Counter schedule_jobs(Job *jobs, unsigned count, unsigned priortiy = PRIORITY_NORMAL);
	void wait_for_counter(Counter& counter, int value);

	void quit();
private:
	


	struct JobBundle
	{
		Job job;
		Counter counter;
	};

	struct WaitingJob
	{
		WaitingJob() : _fiber(nullptr), _counter(nullptr), _value(0) {}
		WaitingJob(void *fiber, AtomicCounter *counter, int value)
			: _fiber(fiber), _counter(counter), _value(value)
		{}
	
		void *_fiber;
		AtomicCounter *_counter;
		int _value;
	};

	moodycamel::ConcurrentQueue<JobBundle>			_jobs[PRIORITY_COUNT];
	//std::vector<WaitingJob>							_wait_list;
	std::list<WaitingJob>							_wait_list;
	moodycamel::BlockingConcurrentQueue<void*>		_fiber_pool;
	moodycamel::BlockingConcurrentQueue<Counter>	_counter_pool;
	
	unsigned _thread_count;
	unsigned _waiting_jobs;
	HANDLE* _threads;
	std::atomic_bool _quit;
	AtomicSpinLock _spin_lock;
	CRITICAL_SECTION _waiting_job_lock;

	void** _switching_fibers;
	void** _counter_waiting_fibers;
	static unsigned long __stdcall ThreadFunction(void *params);
	static void __stdcall FiberFunction(void *params);
	static void __stdcall SwitchFiberFunction(void *params);
	static void __stdcall WaitOnCounterFiberFunction(void *params);
};

struct FiberSystemParams
{
	
	FiberSystem	_fiber_system;
};
}