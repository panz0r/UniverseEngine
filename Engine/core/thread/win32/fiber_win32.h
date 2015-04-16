#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace ue
{
	typedef LPFIBER_START_ROUTINE InternalFiberRoutine;

	inline void* convert_thread_to_fiber(void* params)
	{
		return ConvertThreadToFiber(params);
	}

	inline void convert_fiber_to_thread()
	{
		ConvertFiberToThread();
	}

	inline void switch_to_fiber(void* fiber_handle)
	{
		SwitchToFiber(fiber_handle);
	}

	inline void* get_current_fiber()
	{
		return GetCurrentFiber();
	}

	inline void* create_fiber(size_t stack_size, InternalFiberRoutine routine, void* params)
	{
		return CreateFiber(stack_size, routine, params);
	}

	inline void delete_fiber(void* fiber_handle)
	{
		DeleteFiber(fiber_handle);
	}

}