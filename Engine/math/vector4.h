#pragma once

#include "math.h"
#include "math_types.h"

namespace ue 
{ 

//////////////////////////////////////////////////////////////////////////
// basic vector arithmetic
inline Vector4 negate(Vector4Param iVec);
inline Vector4 add(Vector4Param iVecA, Vector4Param iVecB);
inline Vector4 sub(Vector4Param iVecA, Vector4Param iVecB);
inline Vector4 mul(Vector4Param iVecA, Vector4Param iVecB);
inline Vector4 div(Vector4Param iVecA, Vector4Param iVecB);
inline bool aproxEqual(Vector4Param iVecA, Vector4Param iVecB);
inline bool equal(Vector4Param iVecA, Vector4Param iVecB);

//////////////////////////////////////////////////////////////////////////
// basic vector operators
inline bool operator==(Vector4Param iVecA, Vector4Param iVecB);
inline bool operator!=(Vector4Param iVecA, Vector4Param iVecB);
inline Vector4 operator-(Vector4Param iVec);
inline Vector4 operator+(Vector4Param iVecA, Vector4Param iVecB);
inline Vector4 operator-(Vector4Param iVecA, Vector4Param iVecB);
inline Vector4 operator*(Vector4Param iVecA, Vector4Param iVecB);
inline Vector4 operator/(Vector4Param iVecA, Vector4Param iVecB);
inline Vector4& operator+=(Vector4& ioVecA, Vector4Param iVecB);
inline Vector4& operator-=(Vector4& ioVecA, Vector4Param iVecB);
inline Vector4& operator*=(Vector4& ioVecA, Vector4Param iVecB);
inline Vector4& operator/=(Vector4& ioVecA, Vector4Param iVecB);
inline Vector4 operator*(Vector4Param iVec, float iScalar);
inline Vector4 operator/(Vector4Param iVec, float iScalar);
inline Vector4& operator*=(Vector4& ioVec, float iScalar);
inline Vector4& operator/=(Vector4& ioVec, float iScalar);

//////////////////////////////////////////////////////////////////////////
// factory
inline Vector4 vector4(const float* iData);
inline Vector4 vector4(float iX, float iY, float iZ, float iW);
inline Vector4 vector4(float iScalar);
inline Vector4 vector4(const Vector3& iVec);
inline Vector4 vector4(const Vector3& iVec, float iW);

//////////////////////////////////////////////////////////////////////////
// getters
inline float getComponent(Vector4Param iVec, int iIndex);

//////////////////////////////////////////////////////////////////////////
// extended vector arithmetic
inline float dot(Vector4Param iVecA, Vector4Param iVecB);
inline float lengthSq(Vector4Param iVec);
inline float distanceSq(Vector4Param iVecA, Vector4Param iVecB);
inline float length(Vector4Param iVec);
inline float distance(Vector4Param iVecA, Vector4Param iVecB);
inline Vector4 normalize(Vector4Param iVec);

//////////////////////////////////////////////////////////////////////////
// utility
inline Vector4 splatX(Vector4Param iVec);
inline Vector4 splatY(Vector4Param iVec);
inline Vector4 splatZ(Vector4Param iVec);
inline Vector4 splatW(Vector4Param iVec);


#include "vector4.inl"

} // namespace 

