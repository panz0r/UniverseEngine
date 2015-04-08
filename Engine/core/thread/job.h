#pragma once

namespace em
{

typedef void (*JobFunc)();
typedef void* JobParams;

struct Job
{
	JobFunc job_function;
	JobParams job_params;
};

}