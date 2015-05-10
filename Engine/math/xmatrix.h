#pragma once

#include "math.h"
#include "math_types.h"
#include "xvector.h"

namespace ue
{

//////////////////////////////////////////////////////////////////////////
// factory

inline XMatrix xmatrix(Matrix4x4Param iMat);
inline XMatrix xmatrix(const float* iData);
inline XMatrix xmatrix(float iM00, float iM01, float iM02, float iM03,
                              float iM10, float iM11, float iM12, float iM13,
                              float iM20, float iM21, float iM22, float iM23,
                              float iM30, float iM31, float iM32, float iM33);
inline XMatrix xmatrix(XVectorParam iRow0, XVectorParam iRow1, XVectorParam iRow2, const XVector& iRow3);
inline XMatrix xmatrix(const XVector* iRows);
inline XMatrix xidentity();

inline XMatrix xtranslation(XVectorParam iVec);
inline XMatrix xtranslation(float iX, float iY, float iZ);
inline XMatrix xscaling(XVectorParam iVec);
inline XMatrix xscaling(float iX, float iY, float iZ);
inline XMatrix xrotationX(float iAngle);
inline XMatrix xrotationY(float iAngle);
inline XMatrix xrotationZ(float iAngle);
inline XMatrix xrotationYawPitchRoll(XVectorParam iYawPitchRoll);
inline XMatrix xrotationYawPitchRoll(float iYaw, float iPitch, float iRoll);
inline XMatrix xrotationAxisAngle(XVectorParam iAxis, float iAngle);
inline XMatrix xrotationAxisAngle(float iX, float iY, float iZ, float iAngle);

//////////////////////////////////////////////////////////////////////////
// arithmetics
inline XMatrix mul(XMatrixParam iMatA, XMatrixParam iMatB);
inline XMatrix mul(XMatrixParam iMat, float iScalar);
inline XMatrix& mul(XMatrix& iMat, float iScalar);
inline XMatrix transpose(XMatrixParam iMat);
inline XMatrix inverse(XMatrixParam iMat);
inline bool equal(XMatrixParam iMatA, XMatrixParam iMatB);
inline bool aproxEqual(XMatrixParam iMatA, XMatrixParam iMatB);

//////////////////////////////////////////////////////////////////////////
// transform

inline XVector transform(XMatrixParam iMat, XVectorParam iVec);
inline XVector transformCoord(XMatrixParam iMat, XVectorParam iVec);
inline XVector transformVector(XMatrixParam iMat, XVectorParam iVec);
inline void transformArray(XMatrixParam iMat, XVector* ioArray, unsigned iCount);
inline void transformCoordArray(XMatrixParam iMat, XVector* ioArray, unsigned iCount);
inline void transformVectorArray(XMatrixParam iMat, XVector* ioArray, unsigned iCount);

//////////////////////////////////////////////////////////////////////////
// operators

inline bool operator==(XMatrixParam iMatA, XMatrixParam iMatB);
inline bool operator!=(XMatrixParam iMatA, XMatrixParam iMatB);
inline XMatrix operator*(XMatrixParam iMatA, XMatrixParam iMatB);
inline XMatrix& operator*=(XMatrix& iMatA, XMatrixParam iMatB);
inline XMatrix operator*(XMatrixParam iMatA, float iScalar);
inline XMatrix& operator*=(XMatrix& iMatA, float iScalar);

#if defined(EM_SIMD_IMPL)
	#if (EM_SIMD_IMPL == EM_SIMD_REF)
		#include "ref/xmatrix.inl"
	#elif (EM_SIMD_IMPL == EM_SIMD_SSE)
		#include "sse/xmatrix.inl"
	#endif
#endif

} // namespace


