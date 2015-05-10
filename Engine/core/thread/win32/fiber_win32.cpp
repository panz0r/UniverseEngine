#include "fiber_win32.h"

namespace ue
{

void switch_to_fiber(void* fiber_handle)
{
	SwitchToFiber(fiber_handle);
}

}
