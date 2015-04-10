#pragma once

#include "fiber_pool.h"

namespace em
{

struct Fiber;
struct JobParams;

Fiber* convert_thread_to_fiber(JobParams* params);

}