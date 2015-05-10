#pragma once

#include "math.h"
#include "math_types.h"

namespace ue
{

XPlane xplane(XVectorParam iNormal, XVectorParam iD);
bool intersectPlaneRay(XPlaneParam iPlane, XRayParam iRay, XVector &oIntersectDistance);

#if defined(EM_SIMD_IMPL)
	#if (EM_SIMD_IMPL == EM_SIMD_REF)
		#include "ref/xplane.inl"
	#elif (EM_SIMD_IMPL == EM_SIMD_SSE)
		#include "sse/xplane.inl"
	#endif
#endif

}