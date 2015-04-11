#pragma once

namespace em
{

typedef void (*JobFunc)(void* data);


struct JobDeclaration
{
	JobDeclaration(JobFunc func, void* job_params)
		: _job_function(func), _job_params(job_params), _priority(NORMAL_PRIORITY) {}
	JobDeclaration(JobFunc func, void* job_params, unsigned priority)
		: _job_function(func), _job_params(job_params), _priority(priority) {}

	JobFunc _job_function;
	void* _job_params;
	unsigned _priority;
};

}