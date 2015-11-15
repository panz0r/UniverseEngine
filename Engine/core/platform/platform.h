#pragma once

#if defined(WINDOWSPC)
#include "win32/platform_win32.h"
#else
#error "Unsupported platform"
#endif
