#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <assert.h>

namespace ue
{
	#define DECLARE_THREAD DWORD WINAPI
	typedef LPTHREAD_START_ROUTINE ThreadFunc;
	typedef DWORD_PTR affinity_t;

	inline void create_worker_thread(ThreadFunc func, affinity_t affinity_mask)
	{
		HANDLE thread_handle = CreateThread(NULL, 0, func, NULL, CREATE_SUSPENDED, NULL);
		if(affinity_mask != -1)
		{
			DWORD_PTR old_mask = SetThreadAffinityMask(thread_handle, affinity_mask);
			assert(old_mask != 0 && "failed to set affinity mask");
		}
		ResumeThread(thread_handle);
	}

}