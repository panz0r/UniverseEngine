#if defined(WINDOWSPC)

#define __ALIGN(x) _declspec(align(x))

typedef unsigned long long uint64;
typedef long long int64;

#define __THREAD_LOCAL __declspec(thread)

namespace platform
{

	unsigned hardware_thread_count();

}

#endif