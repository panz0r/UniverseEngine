#pragma once

#if defined(WIN32)
#include "win32/thread_win32.h"
#else
#error "Unsupported platform"
#endif


namespace ue
{



//void create_worker_thread(ThreadFunc func, affinity_t affinity_mask);


}