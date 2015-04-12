#pragma once

namespace em
{

enum { PRIORITY_HIGH, PRIORITY_NORMAL, PRIORITY_LOW };

struct Fiber;
struct JobDeclaration;
class Counter;

void initialize_fiber_system();
void schedule_jobs(JobDeclaration* jobs, int count, Counter** counter, unsigned priority = PRIORITY_NORMAL);
void do_work();
void wait_for_counter(Counter* counter);
void release_fiber(Fiber* fiber);
void switch_fiber();

}