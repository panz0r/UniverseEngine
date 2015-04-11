#pragma once

#include "fiber_pool.h"

namespace em
{

struct Fiber;

Fiber* convert_thread_to_fiber(void* params);

}