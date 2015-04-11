#pragma once

#include "job_declaration.h"
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
		_buffer = (JobDeclaration**)malloc(sizeof(JobDeclaration*)*size);
		unlock(); // initialize lock
	}

	void enqueue(JobDeclaration* job)
	{
		_buffer[_write_head] = job;
		_write_head = (_write_head + 1) % _size;
	}

	const JobDeclaration* dequeue()
	{
		const JobDeclaration* job = _buffer[_read_head];
		_read_head = (_read_head + 1) % _size;
		return job;
	}

	void lock()
	{
		bool val = false;
		while(!_lock.compare_exchange_weak(val, true, std::memory_order_acquire));
	}
	
	void unlock()
	{
		_lock.store(false, std::memory_order_release);
	}

private:
	
	std::atomic<bool> _lock;
	unsigned _size;
	unsigned _read_head;
	unsigned _write_head;
	JobDeclaration** _buffer;
};


}