#pragma once

#if defined(WIN32)
#include "win32/platform_win32.h"
#else
#error "Unsupported platform"
#endif
