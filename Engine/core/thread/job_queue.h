#pragma once

#include <malloc.h>
#include <atomic>

#include "job_declaration.h"
#include "threading.h"

namespace em 
{

__declspec(align(64)) struct JobQueueEntry
{
	JobDeclaration* declaration;
	Counter* counter;

	char padding[64 - sizeof(void*)*2];
};

class JobQueue
{
public:

	JobQueue(unsigned size)
		: _size(size)
		, _read_head(0)
		, _write_head(0)
	{
		size_t job_decl_size = sizeof(JobQueueEntry);
		_buffer = (JobQueueEntry*)_aligned_malloc(job_decl_size*size, 64);
		
	}

	void enqueue(JobDeclaration* job, Counter* counter)
	{
		JobQueueEntry& entry = _buffer[_write_head];
		entry.declaration = job;
		entry.counter = counter;
		if(_write_head >= _read_head) { assert((_write_head + 1) % _size != _read_head); }
		if(_write_head < _read_head) { assert((_write_head + 1) % _size != _read_head); }

		_write_head = (_write_head + 1) % _size;
	}

	bool dequeue(JobDeclaration** job_decl, Counter** counter)
	{
		if(_read_head == _write_head)
			return false;

		JobQueueEntry& entry = _buffer[_read_head];
		*job_decl = entry.declaration;
		*counter = entry.counter;
		_read_head = (_read_head + 1) % _size;
		return true;
	}

	inline void lock()
	{
		fiber_lock(&_lock);
	}
	
	inline void unlock()
	{
		fiber_unlock(&_lock);
	}

private:
	
	std::atomic<bool> _lock;
	unsigned _size;
	unsigned _read_head;
	unsigned _write_head;
	JobQueueEntry* _buffer;
};


}