#pragma once

#include "job.h"
#include <atomic>

namespace em 
{

class JobQueue
{
public:

	JobQueue(unsigned size)
		: _size(size)
		, _read_head(0)
		, _write_head(0)
	{
		_buffer = new Job[size];
		unlock(); // initialize lock
	}

	void enqueue(const Job& job)
	{
		lock();

		memcpy(&_buffer[_write_head], &job, sizeof(Job));
		_write_head = (_write_head + 1) % _size;

		unlock();
	}

	const Job dequeue()
	{
		lock();

		Job job;
		memcpy(&job, &_buffer[_read_head], sizeof(Job));
		_read_head = (_read_head + 1) % _size;

		unlock();
		return job;
	}


private:
	void lock()
	{
		bool val = false;
		while(!_lock.compare_exchange_weak(val, true, std::memory_order_acquire));
	}
	
	void unlock()
	{
		_lock.store(false, std::memory_order_release);
	}
	
	std::atomic<bool> _lock;
	unsigned _size;
	unsigned _read_head;
	unsigned _write_head;
	Job* _buffer;
};


}