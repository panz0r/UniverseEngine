#pragma once

#include <atomic>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace em
{

typedef LPTHREAD_START_ROUTINE ThreadFunc;
typedef DWORD_PTR affinity_t;

struct Fiber;

Fiber* convert_thread_to_fiber(void* params);

void create_worker_thread(ThreadFunc func, affinity_t affinity_mask);


}