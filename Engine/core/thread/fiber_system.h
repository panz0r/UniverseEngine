#pragma once

#include "fiber.h"

namespace ue
{

namespace internal
{
	void CALLBACK fiber_proc(void* fiber_params);
}

enum { PRIORITY_HIGH, PRIORITY_NORMAL, PRIORITY_LOW };

struct JobDeclaration;
class Counter;

void initialize_fiber_system();
void schedule_jobs(JobDeclaration* jobs, int count, Counter** counter, unsigned priority = PRIORITY_NORMAL);
void do_work();
void wait_for_counter(Counter* counter);


}