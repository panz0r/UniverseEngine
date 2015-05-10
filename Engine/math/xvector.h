#pragma once

#include "math.h"
#include "math_types.h"

namespace ue
{

//////////////////////////////////////////////////////////////////////////
// basic vector arithmetic
inline XVector negate(XVectorParam iVec);
inline XVector add(XVectorParam iVecA, XVectorParam iVecB);
inline XVector sub(XVectorParam iVecA, XVectorParam iVecB);
inline XVector mul(XVectorParam iVecA, XVectorParam iVecB);
inline XVector div(XVectorParam iVecA, XVectorParam iVecB);
inline bool aproxEqual(XVectorParam iVecA, XVectorParam iVecB);
inline bool equal(XVectorParam iVecA, XVectorParam iVecB);
inline bool less(XVectorParam iVecA, XVectorParam iVecB);
inline bool lessEqual(XVectorParam iVecA, XVectorParam iVecB);
inline bool greater(XVectorParam iVecA, XVectorParam iVecB);
inline bool greaterEqual(XVectorParam iVecA, XVectorParam iVecB);

//////////////////////////////////////////////////////////////////////////
// basic vector operators

inline XVector operator-(XVectorParam iVec);
inline XVector operator+(XVectorParam iVecA, XVectorParam iVecB);
inline XVector operator-(XVectorParam iVecA, XVectorParam iVecB);
inline XVector operator*(XVectorParam iVecA, XVectorParam iVecB);
inline XVector operator/(XVectorParam iVecA, XVectorParam iVecB);
inline XVector operator+=(XVector& ioVecA, XVectorParam iVecB);
inline XVector operator-=(XVector& ioVecA, XVectorParam iVecB);
inline XVector operator*=(XVector& ioVecA, XVectorParam iVecB);
inline XVector operator/=(XVector& ioVecA, XVectorParam iVecB);
inline XVector operator*(XVectorParam iVec, float iScalar);
inline XVector operator/(XVectorParam iVec, float iScalar);
inline XVector operator*=(XVector& ioVec, float iScalar);
inline XVector operator/=(XVector& ioVec, float iScalar);

//////////////////////////////////////////////////////////////////////////
// factory

inline XVector xvector(Vector2Param iVec);
inline XVector xvector(Vector3Param iVec);
inline XVector xvector(Vector4Param iVec);
inline XVector xvector(const float* iData);
inline XVector xvector(float iX, float iY, float iZ, float iW);
inline XVector xvector(float iScalar);

//////////////////////////////////////////////////////////////////////////
// setters

inline XVector setX(XVectorParam iVec, float iX);
inline XVector setY(XVectorParam iVec, float iY);
inline XVector setZ(XVectorParam iVec, float iZ);
inline XVector setW(XVectorParam iVec, float iW);

//////////////////////////////////////////////////////////////////////////
// getters

inline float getX(XVectorParam iVec);
inline float getY(XVectorParam iVec);
inline float getZ(XVectorParam iVec);
inline float getW(XVectorParam iVec);
inline float getComponent(XVectorParam iVec, int iIndex);

//////////////////////////////////////////////////////////////////////////
// extended vector arithmetic

inline float dot(XVectorParam iVecA, XVectorParam iVecB);
inline float lengthSq(XVectorParam iVec);
inline float distanceSq(XVectorParam iVecA, XVectorParam iVecB);
inline float length(XVectorParam iVec);
inline float distance(XVectorParam iVecA, XVectorParam iVecB);
inline XVector dotVector(XVectorParam iVecA, XVectorParam iVecB);
inline XVector lengthSqVector(XVectorParam iVec);
inline XVector distanceSqVector(XVectorParam iVecA, XVectorParam iVecB);
inline XVector lengthVector(XVectorParam iVec);
inline XVector distanceVector(XVectorParam iVecA, XVectorParam iVecB);
inline XVector normalize(XVectorParam iVec);
inline XVector cross(XVectorParam iVecA, XVectorParam iVecB);
inline float sqrt(XVectorParam iVec);
inline float rsqrt(XVectorParam iVec);
inline XVector sqrtVector(XVectorParam iVec);
inline XVector rsqrtVector(XVectorParam iVec);

//////////////////////////////////////////////////////////////////////////
// utility

inline XVector splatX(XVectorParam iVec);
inline XVector splatY(XVectorParam iVec);
inline XVector splatZ(XVectorParam iVec);
inline XVector splatW(XVectorParam iVec);

#if defined(EM_SIMD_IMPL)
	#if (EM_SIMD_IMPL == EM_SIMD_REF)
		#include "ref/xvector.inl"
	#elif (EM_SIMD_IMPL == EM_SIMD_SSE)
		#include "sse/xvector.inl"
	#endif
#endif

} // namespace

