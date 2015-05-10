#pragma once

#if defined(WIN32)
#include "win32/fiber_win32.h"
#else
#error "Unsupported Platform"
#endif
