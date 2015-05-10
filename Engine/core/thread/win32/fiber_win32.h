#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace ue
{
	typedef LPFIBER_START_ROUTINE InternalFiberRoutine;

	inline void* convert_thread_to_fiber(void* params)
	{
		return ConvertThreadToFiberEx(params, FIBER_FLAG_FLOAT_SWITCH);
	}

	inline void convert_fiber_to_thread()
	{
		ConvertFiberToThread();
	}

	__declspec(noinline) void switch_to_fiber(void* fiber_handle);

	inline void* get_current_fiber()
	{
		return GetCurrentFiber();
	}

	inline void* create_fiber(size_t stack_size, InternalFiberRoutine routine, void* params)
	{
		return CreateFiberEx(stack_size, 0, FIBER_FLAG_FLOAT_SWITCH, routine, params);
	}

	inline void delete_fiber(void* fiber_handle)
	{
		DeleteFiber(fiber_handle);
	}

}