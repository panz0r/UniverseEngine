#if defined(WIN32)
#include "platform_win32.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace platform
{
	unsigned hardware_thread_count()
	{
		return 4; // TODO
	}
}


#endif
