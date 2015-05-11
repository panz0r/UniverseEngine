#pragma once

#include <core/thread/fiber_system.h>

namespace ue
{

struct RaytraceParams
{
	unsigned width, height;
	char* pixels;
};

DECLARE_JOB(start_job);

}