#pragma once


#include <memory>
#include <vector>
#include <concurrentqueue/concurrentqueue.h>
#include <concurrentqueue/blockingconcurrentqueue.h>


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
	FiberSystemParams *thread_system_params;
	unsigned thread_id;
};

typedef std::atomic_uint32_t AtomicCounter;
typedef std::shared_ptr<AtomicCounter> Counter;

class FiberSystem
{
public:
	FiberSystem();
	~FiberSystem();

	void initialize(unsigned thread_count, unsigned fiber_pool_count);
		
	Counter schedule_jobs(Job *jobs, unsigned count, unsigned priortiy = PRIORITY_NORMAL);
	void wait_for_counter(Counter& counter, unsigned value);
private:


	struct JobBundle
	{
		Job job;
		AtomicCounter* counter;
	};

	struct WaitingJob
	{
		WaitingJob() : _fiber(nullptr), _counter(nullptr), _value(0) {}
		WaitingJob(void *fiber, AtomicCounter *counter, unsigned value)
			: _fiber(fiber), _counter(counter), _value(value)
		{}
	
		void *_fiber;
		AtomicCounter *_counter;
		unsigned _value;
	};

	moodycamel::ConcurrentQueue<JobBundle>			_jobs[PRIORITY_COUNT];
	std::vector<WaitingJob>							_wait_list;
	moodycamel::BlockingConcurrentQueue<void*>		_fiber_pool;

	HANDLE* _threads;
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