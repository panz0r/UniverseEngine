#pragma once

#include "math.h"
#include "math_types.h"
#include "vector4.h"

namespace ue
{

//////////////////////////////////////////////////////////////////////////
// factory

inline Matrix4x4 matrix4x4(const float* iData);
inline Matrix4x4 matrix4x4(float iM00, float iM01, float iM02, float iM03,
                              float iM10, float iM11, float iM12, float iM13,
                              float iM20, float iM21, float iM22, float iM23,
                              float iM30, float iM31, float iM32, float iM33);
inline Matrix4x4 matrix4x4(Vector4Param iRow0, Vector4Param iRow1, Vector4Param iRow2, Vector4Param iRow3);
inline Matrix4x4 matrix4x4(const Vector4* iRows);
inline Matrix4x4 matrix4x4(const Matrix3x3Param& iMat3);
inline Matrix4x4 identity4x4();

inline Matrix4x4 translation(Vector4Param iVec);
inline Matrix4x4 translation(float iX, float iY, float iZ);

//inline Matrix4x4 scaling4x4(Vector4Param iVec);
//inline Matrix4x4 scaling4x4(float iX, float iY, float iZ);
//inline Matrix4x4 rotationX4x4(float iAngle);
//inline Matrix4x4 rotationY4x4(float iAngle);
//inline Matrix4x4 rotationZ4x4(float iAngle);
//inline Matrix4x4 rotationYawPitchRoll(Vector4Param iYawPitchRoll);
//inline Matrix4x4 rotationYawPitchRoll(float iYaw, float iPitch, float iRoll);
//inline Matrix4x4 rotationAxisAngle(Vector4Param iAxis, float iAngle);
//inline Matrix4x4 rotationAxisAngle(float iX, float iY, float iZ, float iAngle);

//////////////////////////////////////////////////////////////////////////
// arithmetics
inline Matrix4x4 mul(Matrix4x4Param iMatA, Matrix4x4Param iMatB);
inline Matrix4x4 mul(Matrix4x4Param iMat, float iScalar);
inline Matrix4x4& mul(Matrix4x4& iMat, float iScalar);
inline Matrix4x4 transpose(Matrix4x4Param iMat);
inline Matrix4x4 inverse(Matrix4x4Param iMat);
inline bool equal(Matrix4x4Param iMatA, Matrix4x4Param iMatB);
inline bool aproxEqual(Matrix4x4Param iMatA, Matrix4x4Param iMatB);

//////////////////////////////////////////////////////////////////////////
// transform

inline Vector4 transform(Matrix4x4Param iMat, Vector4Param iVec);
inline Vector4 transformCoord(Matrix4x4Param iMat, Vector4Param iVec);
inline Vector4 transformVector(Matrix4x4Param iMat, Vector4Param iVec);
inline Vector3 transformCoord(Matrix4x4Param iMat, Vector3Param iVec);
inline Vector3 transformVector(Matrix4x4Param iMat, Vector3Param iVec);
inline void transformArray(Matrix4x4Param iMat, Vector4* ioArray, unsigned iCount);
inline void transformCoordArray(Matrix4x4Param iMat, Vector4* ioArray, unsigned iCount);
inline void transformVectorArray(Matrix4x4Param iMat, Vector4* ioArray, unsigned iCount);

//////////////////////////////////////////////////////////////////////////
// utility

inline Matrix4x4 lookAtLH(Vector4Param iEyePoint, Vector4Param iFocusPoint, Vector4Param iUpVec);
//inline Matrix4x4 lookAtRH(Vector4Param iEyePoint, Vector4Param iFocusPoint, Vector4Param iUpVec);
inline Matrix4x4 lookAtRH(Vector3Param iEyePoint, Vector3Param iFocusPoint, Vector3Param iUpVec);

inline Matrix4x4 perspectiveFovLH(float iFovDeg, float iAspect, float iNear, float iFar);
inline Matrix4x4 perspectiveFovRH(float iFovDeg, float iAspect, float iNear, float iFar);
inline Matrix4x4 orthographicLH(float iWidth, float iHeight, float iNear, float iFar);
inline Matrix4x4 orthographicRH(float iWidth, float iHeight, float iNear, float iFar);
inline Matrix4x4 orthographicOffCenterLH(float iLeft, float iRight, float iTop, float iBottom, float iNear, float iFar);
inline Matrix4x4 orthographicOffCenterRH(float iLeft, float iRight, float iTop, float iBottom, float iNear, float iFar);

//////////////////////////////////////////////////////////////////////////
// operators

inline bool operator==(Matrix4x4Param iMatA, Matrix4x4Param iMatB);
inline bool operator!=(Matrix4x4Param iMatA, Matrix4x4Param iMatB);
inline Matrix4x4 operator*(Matrix4x4Param iMatA, Matrix4x4Param iMatB);
inline Matrix4x4& operator*=(Matrix4x4& iMatA, Matrix4x4Param iMatB);
inline Matrix4x4 operator*(Matrix4x4Param iMatA, float iScalar);
inline Matrix4x4& operator*=(Matrix4x4& iMatA, float iScalar);


#include "matrix4.inl"

} // namespace
