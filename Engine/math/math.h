#pragma once

#include <math.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////

typedef __int64 int64;
typedef unsigned __int64 uint64;


#define EM_SIMD_REF 0
#define EM_SIMD_SSE 1
#define EM_SIMD_NEON 2

#ifndef EM_SIMD_IMPL
#define EM_SIMD_IMPL EM_SIMD_SSE
#endif

#if !defined(EM_SIMD_IMPL)
#error need to choose a simd math implementation!
#endif

//////////////////////////////////////////////////////////////////////////

#undef min
#undef max

//////////////////////////////////////////////////////////////////////////

namespace ue
{


template <typename T> struct RemoveConst { typedef T result; };
template <typename T> struct RemoveConst<const T> { typedef T result; };
template <typename T> struct RemoveConst<const T&> { typedef T result; };
template <typename T> struct RemoveConst<const T*> { typedef T result; };

//////////////////////////////////////////////////////////////////////////

class Math
{
public:
	static const float kPI;
	static const float kRAD_TO_DEG;
	static const float kDEG_TO_RAD;
	static const float kFLOAT_MIN;
	static const float kFLOAT_MAX;
	static const float kFLOAT_EPS;
	static const int kINT_MIN;
	static const int kINT_MAX;
	

	inline static float randf()
	{
		return rand()%RAND_MAX / (float)RAND_MAX;
	}

	template <typename T>
	inline static T radians(T iDegrees)
	{
		return iDegrees * (T)kDEG_TO_RAD;
	}

	template <typename T>
	inline static T degrees(T iRadians)
	{
		return iRadians * (T)kRAD_TO_DEG;
	}

	template <typename T> 
	inline static T min(T iValue1, T iValue2)
	{
		return (iValue1 < iValue2) ? iValue1 : iValue2; 
	}

	template <typename T> 
	inline static T max(T iValue1, T iValue2)
	{
		return (iValue1 > iValue2) ? iValue1 : iValue2; 
	}

	template <typename T>
	inline static T mix(T iValue1, T iValue2, T iMix)
	{
		return iValue2 * iMix + iValue1 * ((T)1 - iMix);
	}

	template <typename T>
	inline static T clamp(T iValue, T iMin, T iMax)
	{
		return iValue < iMin ? iMin : iValue > iMax ? iMax : iValue;
	}
	
	template <typename T>
	inline static T saturate(T iValue)
	{
		return clamp<T>(iValue, (T)0, (T)1);
	}

	//inline static float abs(float iValue)
	//{
	//	return ::fabs(iValue);
	//}

	//inline static double abs(double iValue)
	//{
	//	return ::abs(iValue);
	//}

	template <typename T>
	inline static T smoothstep(T iEdge0, T iEdge1, T iAmount)
	{
		float x = saturate<T>((iAmount - iEdge0) / (iEdge1 - iEdge0));
		return x*x*((T)3 - (T)2*x);
	}
};

} // namespace em
