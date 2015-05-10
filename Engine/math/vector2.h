#pragma once

#include "math.h"
#include "math_types.h"

namespace ue 
{ 

//////////////////////////////////////////////////////////////////////////
// basic vector arithmetic
inline Vector2 add(Vector2Param iVecA, Vector2Param iVecB);
inline Vector2 sub(Vector2Param iVecA, Vector2Param iVecB);
inline Vector2 mul(Vector2Param iVecA, Vector2Param iVecB);
inline Vector2 div(Vector2Param iVecA, Vector2Param iVecB);
inline bool aproxEqual(Vector2Param iVecA, Vector2Param iVecB);
inline bool equal(Vector2Param iVecA, Vector2Param iVecB);

//////////////////////////////////////////////////////////////////////////
// basic vector operators
inline bool operator==(Vector2Param iVecA, Vector2Param iVecB);
inline bool operator!=(Vector2Param iVecA, Vector2Param iVecB);
inline Vector2 operator-(Vector2Param iVec);
inline Vector2 operator+(Vector2Param iVecA, Vector2Param iVecB);
inline Vector2 operator-(Vector2Param iVecA, Vector2Param iVecB);
inline Vector2 operator*(Vector2Param iVecA, Vector2Param iVecB);
inline Vector2 operator/(Vector2Param iVecA, Vector2Param iVecB);
inline Vector2& operator+=(Vector2& ioVecA, Vector2Param iVecB);
inline Vector2& operator-=(Vector2& ioVecA, Vector2Param iVecB);
inline Vector2& operator*=(Vector2& ioVecA, Vector2Param iVecB);
inline Vector2& operator/=(Vector2& ioVecA, Vector2Param iVecB);
inline Vector2 operator*(Vector2Param iVec, float iScalar);
inline Vector2 operator/(Vector2Param iVec, float iScalar);
inline Vector2& operator*=(Vector2& ioVec, float iScalar);
inline Vector2& operator/=(Vector2& ioVec, float iScalar);

//////////////////////////////////////////////////////////////////////////
// factory
inline Vector2 vector2(const float* iData);
inline Vector2 vector2(float iX, float iY);
inline Vector2 vector2(float iScalar);

//////////////////////////////////////////////////////////////////////////
// getters
inline float getComponent(Vector2Param iVec, int iIndex);

//////////////////////////////////////////////////////////////////////////
// extended vector arithmetic
inline float dot(Vector2Param iVecA, Vector2Param iVecB);
inline float lengthSq(Vector2Param iVec);
inline float distanceSq(Vector2Param iVecA, Vector2Param iVecB);
inline float length(Vector2Param iVec);
inline float distance(Vector2Param iVecA, Vector2Param iVecB);
inline Vector2 normalize(Vector2Param iVec);

//////////////////////////////////////////////////////////////////////////
// utility
inline Vector2 splatX(Vector2Param iVec);
inline Vector2 splatY(Vector2Param iVec);

#include "vector2.inl"

} // namespace 

