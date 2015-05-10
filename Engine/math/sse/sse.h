#pragma once

#include <xmmintrin.h>
#include <emmintrin.h>

// TODO, something something
#define SSE_VERSION 3

#if defined(SSE_VERSION) && SSE_VERSION >= 3
#include <intrin.h>
#endif

// An easier to wrap your barin around version of __MM_SHUFFLE()
// EM_SHUFFLE(a, b, c, d)
//  a: index of arg0 and outputs to index0
//  b: index of arg0 and outputs to index1
//  c: index of arg1 and outputs to index2
//  d: index of arg1 and outputs to index3

#define EM_SHUFFLE(a,b,c,d) \
	(((d)<<6) | ((c)<<4) | (((b)<<2) | (a)))


#define EM_TRANSPOSE4_PS(row0, row1, row2, row3) {        \
	__m128 tmp3, tmp2, tmp1, tmp0;                          \
	\
	tmp0   = _mm_shuffle_ps((row0), (row1), 0x44);          \
	tmp2   = _mm_shuffle_ps((row0), (row1), 0xEE);          \
	tmp1   = _mm_shuffle_ps((row2), (row3), 0x44);          \
	tmp3   = _mm_shuffle_ps((row2), (row3), 0xEE);          \
	\
	(row0) = _mm_shuffle_ps(tmp0, tmp1, 0x88);              \
	(row1) = _mm_shuffle_ps(tmp0, tmp1, 0xDD);              \
	(row2) = _mm_shuffle_ps(tmp2, tmp3, 0x88);              \
	(row3) = _mm_shuffle_ps(tmp2, tmp3, 0xDD);              \
  }


namespace ue
{
	typedef __m128 XVector;
	typedef XVector XVectorParam;

	typedef __m128 XQuaternion;
	typedef XQuaternion XQuaternionParam;

	__declspec(align(16)) struct XVectorConvert
	{
		union 
		{
			XVector v;
			float f[4];
			int64 i64[2];
			int i32[4];
		};
	};

	__declspec(align(16)) struct XMatrix
	{
		XVector row[4];
	};
	typedef const XMatrix& XMatrixParam;


	__declspec(align(16)) struct XSphere
	{
		XVector center;
		XVector radiusSq;
	};
	typedef const XSphere& XSphereParam;

	__declspec(align(16)) struct XPlane
	{
		XVector normal;
		XVector d;
	};
	typedef const XPlane& XPlaneParam;

	__declspec(align(16)) struct XRay
	{
		XVector origin;
		XVector direction;
	};
	typedef const XRay& XRayParam;

}