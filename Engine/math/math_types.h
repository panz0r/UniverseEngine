#pragma once

#include <xmmintrin.h>
#include <emmintrin.h>

// include based on simd implementation
#if defined(EM_SIMD_IMPL)
	#if (EM_SIMD_IMPL == EM_SIMD_REF)
		#include "ref/ref.h
	#elif (EM_SIMD_IMPL == EM_SIMD_SSE)
		#include "sse/sse.h"
	#else
    #error unsupported!
  #endif
#endif

namespace ue
{

struct Vector2
{
	union
	{
		struct
		{
			float x,y;
		};
		float v[2];
	};
	static const Vector2 kZero;
	static const Vector2 kOne;
	static const Vector2 kHalf;
	static const Vector2 kNegOne;
	static const Vector2 kUnitX;
	static const Vector2 kUnitY;
};

typedef const Vector2& Vector2Param;

struct Vector3
{
	union
	{
		struct
		{
			float x,y,z;
		};
		float v[3];
	};

	static const Vector3 kZero;
	static const Vector3 kOne;
	static const Vector3 kHalf;
	static const Vector3 kNegOne;
	static const Vector3 kUnitX;
	static const Vector3 kUnitY;
	static const Vector3 kUnitZ;

};

typedef const Vector3& Vector3Param;


struct Vector4
{
	union
	{
		struct
		{
			float x,y,z,w;
		};
		float v[4];
	};

	static const Vector4 kZero;
	static const Vector4 kOne;
	static const Vector4 kHalf;
	static const Vector4 kNegOne;
	static const Vector4 kUnitX;
	static const Vector4 kUnitY;
	static const Vector4 kUnitZ;
	static const Vector4 kUnitW;
};

typedef const Vector4& Vector4Param;


struct Matrix3x3
{
	union
	{
		Vector3 row[3];
		float v[9];
		struct
		{
		  float m00,m01,m02;
		  float m10,m11,m12;
		  float m20,m21,m22;
		};
	};
};

typedef const Matrix3x3& Matrix3x3Param;

__declspec(align(16)) struct Matrix4x4
{
	union
	{
		Vector4 row[4];
		float v[16];
		struct
		{
		  float m00,m01,m02,m03;
		  float m10,m11,m12,m13;
		  float m20,m21,m22,m23;
		  float m30,m31,m32,m33;
		};
	};
};

typedef const Matrix4x4& Matrix4x4Param;

__declspec(align(16)) struct Quaternion
{
	float x,y,z,w;
};

typedef const Quaternion& QuaternionParam;


// Gometry

struct Ray
{
	Vector3 origin;
	Vector3 direction;
};

typedef const Ray& RayParam;

struct Sphere
{
	Vector3	center;
	float radius;
	float radiusSq;
};

typedef const Sphere& SphereParam;



struct Plane
{
	Vector3 normal;
	float d;
};

typedef const Plane& PlaneParam;



}