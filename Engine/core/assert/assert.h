#pragma once

#include <assert.h>

namespace ue
{

__forceinline void internal_assert(const char *expression, const char *msg, ...)
{
	// TODO
	assert(false);

}


//#define UASSERT(EXPR, MSG, ...) while( !(EXPR) ) { internal_assert(#EXPR, MSG, ##__VA_ARGS__); }
//#define UENSURE(EXPR) while(!(EXPR)) { internal_assert(#EXPR, "Ensure Failed"); }

#define UASSERT(EXPR, MSG, ...) assert(EXPR)
#define UENSURE(EXPR) assert(EXPR)


}