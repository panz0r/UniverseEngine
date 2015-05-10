#pragma once

#include "math.h"
#include "math_types.h"

namespace ue
{

XRay xray(XVectorParam iOrigin, XVectorParam iDirection);

#if defined(EM_SIMD_IMPL)
	#if (EM_SIMD_IMPL == EM_SIMD_REF)
		#include "ref/xray.inl"
	#elif (EM_SIMD_IMPL == EM_SIMD_SSE)
		#include "sse/xray.inl"
	#endif
#endif

}