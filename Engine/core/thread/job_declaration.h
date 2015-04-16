#pragma once

namespace ue
{

typedef void (*JobFunc)(void* data);


struct JobDeclaration
{
	JobDeclaration() {}
	JobDeclaration(JobFunc func, void* job_params)
		: _job_function(func), _job_params(job_params) {}

	JobFunc _job_function;
	void* _job_params;
};

}