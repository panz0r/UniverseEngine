#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <assert.h>

namespace ue
{
	#define DECLARE_THREAD DWORD WINAPI
	typedef LPTHREAD_START_ROUTINE ThreadFunc;
	typedef DWORD_PTR affinity_t;
	
	inline void set_thread_affinity_mask(HANDLE thread_handle, affinity_t affinity_mask)
	{
		if(affinity_mask != -1) {
			DWORD_PTR old_mask = SetThreadAffinityMask(thread_handle, affinity_mask);
			assert(old_mask != 0 && "failed to set affinity mask");
		}
	}

	inline HANDLE create_worker_thread(ThreadFunc func, affinity_t affinity_mask, void *params)
	{
		HANDLE thread_handle = CreateThread(NULL, 0, func, params, CREATE_SUSPENDED, NULL);
		set_thread_affinity_mask(thread_handle, affinity_mask);
		ResumeThread(thread_handle);
		return thread_handle;
	}

	inline HANDLE get_current_thread()
	{
		return GetCurrentThread();
	}

}