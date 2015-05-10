#pragma once

#include "math.h"
#include "math_types.h"

namespace ue 
{ 

//////////////////////////////////////////////////////////////////////////
// basic vector arithmetic
inline Vector3 add(Vector3Param iVecA, Vector3Param iVecB);
inline Vector3 sub(Vector3Param iVecA, Vector3Param iVecB);
inline Vector3 mul(Vector3Param iVecA, Vector3Param iVecB);
inline Vector3 div(Vector3Param iVecA, Vector3Param iVecB);
inline Vector3 mul(Vector3Param iVecA, float iScalar);
inline Vector3 div(Vector3Param iVecA, float iScalar);
inline bool aproxEqual(Vector3Param iVecA, Vector3Param iVecB, float iEpsilon = Math::kFLOAT_EPS);
inline bool equal(Vector3Param iVecA, Vector3Param iVecB);

//////////////////////////////////////////////////////////////////////////
// basic vector operators
inline bool operator==(Vector3Param iVecA, Vector3Param iVecB);
inline bool operator!=(Vector3Param iVecA, Vector3Param iVecB);
inline Vector3 operator-(Vector3Param iVec);
inline Vector3 operator+(Vector3Param iVecA, Vector3Param iVecB);
inline Vector3 operator-(Vector3Param iVecA, Vector3Param iVecB);
inline Vector3 operator*(Vector3Param iVecA, Vector3Param iVecB);
inline Vector3 operator/(Vector3Param iVecA, Vector3Param iVecB);
inline Vector3& operator+=(Vector3& ioVecA, Vector3Param iVecB);
inline Vector3& operator-=(Vector3& ioVecA, Vector3Param iVecB);
inline Vector3& operator*=(Vector3& ioVecA, Vector3Param iVecB);
inline Vector3& operator/=(Vector3& ioVecA, Vector3Param iVecB);
inline Vector3 operator*(Vector3Param iVec, float iScalar);
inline Vector3 operator/(Vector3Param iVec, float iScalar);
inline Vector3 operator*(float iScalar, Vector3Param iVec);
inline Vector3 operator/(float iScalar, Vector3Param iVec);
inline Vector3& operator*=(Vector3& ioVec, float iScalar);
inline Vector3& operator/=(Vector3& ioVec, float iScalar);

//////////////////////////////////////////////////////////////////////////
// factory
inline Vector3 vector3(Vector2Param iVec);
inline Vector3 vector3(Vector4Param iVec);
inline Vector3 vector3(const float* iData);
inline Vector3 vector3(float iX, float iY, float iZ);
inline Vector3 vector3(float iScalar);

//////////////////////////////////////////////////////////////////////////
// getters
inline float getComponent(Vector3Param iVec, int iIndex);

//////////////////////////////////////////////////////////////////////////
// extended vector arithmetic
inline float dot(Vector3Param iVecA, Vector3Param iVecB);
inline float lengthSq(Vector3Param iVec);
inline float distanceSq(Vector3Param iVecA, Vector3Param iVecB);
inline float length(Vector3Param iVec);
inline float distance(Vector3Param iVecA, Vector3Param iVecB);
inline Vector3 normalize(Vector3Param iVec);
inline Vector3 normalize(Vector3Param iVec, float& oLength);
inline Vector3 cross(Vector3Param iVecA, Vector3Param iVecB);
inline Vector3 reflect(Vector3Param iVec, Vector3Param iNormal);
inline Vector3 refract(Vector3Param iVec, Vector3Param iNormal, float iIndexOfRefraction);

//////////////////////////////////////////////////////////////////////////
// utility
inline Vector3 splatX(Vector3Param iVec);
inline Vector3 splatY(Vector3Param iVec);
inline Vector3 splatZ(Vector3Param iVec);

#include "vector3.inl"

} // namespace 

