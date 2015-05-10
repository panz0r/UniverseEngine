#pragma once

#include "math.h"
#include "math_types.h"
#include "vector3.h"

namespace ue
{

//////////////////////////////////////////////////////////////////////////
// factory

inline Matrix3x3 matrix3x3(Matrix4x4Param iMat);
inline Matrix3x3 matrix3x3(const float* iData);
inline Matrix3x3 matrix3x3(float iM00, float iM01, float iM02,
                              float iM10, float iM11, float iM12,
                              float iM20, float iM21, float iM22,
                              float iM30, float iM31, float iM32);
inline Matrix3x3 matrix3x3(Vector3Param iRow0, Vector3Param iRow1, Vector3Param iRow2);
inline Matrix3x3 matrix3x3(const Vector3* iRows);
inline Matrix3x3 identity3x3();

inline Matrix3x3 scaling(Vector3Param iVec);
inline Matrix3x3 scaling(float iX, float iY, float iZ);
inline Matrix3x3 rotationX(float iAngle);
inline Matrix3x3 rotationY(float iAngle);
inline Matrix3x3 rotationZ(float iAngle);
inline Matrix3x3 rotationYawPitchRoll(Vector3Param iYawPitchRoll);
inline Matrix3x3 rotationYawPitchRoll(float iYaw, float iPitch, float iRoll);
inline Matrix3x3 rotationAxisAngle(Vector3Param iAxis, float iAngle);
inline Matrix3x3 rotationAxisAngle(float iX, float iY, float iZ, float iAngle);

//////////////////////////////////////////////////////////////////////////
// arithmetics
inline Matrix3x3 mul(Matrix3x3Param iMatA, Matrix3x3Param iMatB);
inline Matrix3x3 mul(Matrix3x3Param iMat, float iScalar);
inline Matrix3x3& mul(Matrix3x3& iMat, float iScalar);
inline Matrix3x3 transpose(Matrix3x3Param iMat);
inline Matrix3x3 inverse(Matrix3x3Param iMat);
inline bool equal(Matrix3x3Param iMatA, Matrix3x3Param iMatB);
inline bool aproxEqual(Matrix3x3Param iMatA, Matrix3x3Param iMatB);

//////////////////////////////////////////////////////////////////////////
// transform

inline Vector3 transformVector(Matrix3x3Param iMat, Vector3Param iVec);
inline void transformVectorArray(Matrix3x3Param iMat, Vector3* ioArray, unsigned iCount);

//////////////////////////////////////////////////////////////////////////
// operators

inline bool operator==(Matrix3x3Param iMatA, Matrix3x3Param iMatB);
inline bool operator!=(Matrix3x3Param iMatA, Matrix3x3Param iMatB);
inline Matrix3x3 operator*(Matrix3x3Param iMatA, Matrix3x3Param iMatB);
inline Matrix3x3& operator*=(Matrix3x3& iMatA, Matrix3x3Param iMatB);
inline Matrix3x3 operator*(Matrix3x3Param iMatA, float iScalar);
inline Matrix3x3& operator*=(Matrix3x3& iMatA, float iScalar);

#include "matrix3.inl"

} // namespace
