#pragma once

#include "math.h"
#include "math_types.h"

namespace ue
{

inline XSphere xsphere();
inline XSphere xsphere(XVectorParam iCenter, XVectorParam iRadius);
inline XSphere xsphere(float iX, float iY, float iZ, float iRadius);

inline bool intersectSphereRay(XSphereParam iSphere, XRayParam iRay, XVector& oIntersectDistance0, XVector& oIntersectDistance1);

#if defined(EM_SIMD_IMPL)
	#if (EM_SIMD_IMPL == EM_SIMD_REF)
		#include "ref/xsphere.inl"
	#elif (EM_SIMD_IMPL == EM_SIMD_SSE)
		#include "sse/xsphere.inl"
	#endif
#endif

}