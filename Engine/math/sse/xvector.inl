
//////////////////////////////////////////////////////////////////////////
// Basic vector arithmetic

inline XVector negate(XVectorParam iVec)
{
	static const __m128 kSignMask = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));
	return _mm_xor_ps(iVec, kSignMask);
}

//////////////////////////////////////////////////////////////////////////

inline XVector add(XVectorParam iVecA, XVectorParam iVecB)
{
	return _mm_add_ps(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline XVector sub(XVectorParam iVecA, XVectorParam iVecB)
{
	return _mm_sub_ps(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline XVector mul(XVectorParam iVecA, XVectorParam iVecB)
{
	return _mm_mul_ps(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline XVector div(XVectorParam iVecA, XVectorParam iVecB)
{
	return _mm_div_ps(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline bool aproxEqual(XVectorParam iVecA, XVectorParam iVecB)
{
	// todo
	return true;
}

//////////////////////////////////////////////////////////////////////////

inline bool equal(XVectorParam iVecA, XVectorParam iVecB)
{
	XVectorConvert result;
	result.v = _mm_cmpeq_ps(iVecA, iVecB);
	return (result.i64[0] != 0) && 
		(result.i64[1] != 0);
}

//////////////////////////////////////////////////////////////////////////

inline bool less(XVectorParam iVecA, XVectorParam iVecB)
{
	XVectorConvert result;
	result.v = _mm_cmplt_ss(iVecA, iVecB);
	return result.i32[0] != 0;
}

//////////////////////////////////////////////////////////////////////////

inline bool lessEqual(XVectorParam iVecA, XVectorParam iVecB)
{
	XVectorConvert result;
	result.v = _mm_cmple_ss(iVecA, iVecB);
	return result.i32[0] != 0;
}

//////////////////////////////////////////////////////////////////////////

inline bool greater(XVectorParam iVecA, XVectorParam iVecB)
{
	XVectorConvert result;
	result.v = _mm_cmpgt_ss(iVecA, iVecB);
	return result.i32[0] != 0;
}

//////////////////////////////////////////////////////////////////////////

inline bool greaterEqual(XVectorParam iVecA, XVectorParam iVecB)
{
	XVectorConvert result;
	result.v = _mm_cmpge_ss(iVecA, iVecB);
	return result.i32[0] != 0;
}

//////////////////////////////////////////////////////////////////////////
// operators

inline XVector operator-(XVectorParam iVec)
{
	return negate(iVec);
}

//////////////////////////////////////////////////////////////////////////

inline XVector operator+(XVectorParam iVecA, XVectorParam iVecB)
{
	return add(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline XVector operator-(XVectorParam iVecA, XVectorParam iVecB)
{
	return sub(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline XVector operator*(XVectorParam iVecA, XVectorParam iVecB)
{
	return mul(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline XVector operator/(XVectorParam iVecA, XVectorParam iVecB)
{
	return div(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline XVector operator+=(XVector& ioVecA, XVectorParam iVecB)
{
	ioVecA = add(ioVecA, iVecB);
	return ioVecA;
}

//////////////////////////////////////////////////////////////////////////

inline XVector operator-=(XVector& ioVecA, XVectorParam iVecB)
{
	ioVecA = sub(ioVecA, iVecB);
	return ioVecA;
}

//////////////////////////////////////////////////////////////////////////

inline XVector operator*=(XVector& ioVecA, XVectorParam iVecB)
{
	ioVecA = mul(ioVecA, iVecB);
	return ioVecA;
}

//////////////////////////////////////////////////////////////////////////

inline XVector operator/=(XVector& ioVecA, XVectorParam iVecB)
{
	ioVecA = div(ioVecA, iVecB);
	return ioVecA;
}

//////////////////////////////////////////////////////////////////////////

inline XVector operator*(XVectorParam iVec, float iScalar)
{
	return mul(iVec, xvector(iScalar));
}

//////////////////////////////////////////////////////////////////////////

inline XVector operator/(XVectorParam iVec, float iScalar)
{
	return div(iVec, xvector(iScalar));
}

//////////////////////////////////////////////////////////////////////////

inline XVector operator*=(XVector& ioVec, float iScalar)
{
	ioVec = mul(ioVec, xvector(iScalar));
	return ioVec;
}

//////////////////////////////////////////////////////////////////////////

inline XVector operator/=(XVector& ioVec, float iScalar)
{
	ioVec = div(ioVec, xvector(iScalar));
	return ioVec;
}

//////////////////////////////////////////////////////////////////////////
// factory

inline XVector xvector(Vector2Param iVec)
{
	return xvector(iVec.x, iVec.y, 0.f, 0.f);
}

//////////////////////////////////////////////////////////////////////////

inline XVector xvector(Vector3Param iVec)
{
	return xvector(iVec.x, iVec.y, iVec.z, 0.f);
}

//////////////////////////////////////////////////////////////////////////

inline XVector xvector(Vector4Param iVec)
{
	return xvector(iVec.v);
}

//////////////////////////////////////////////////////////////////////////

inline XVector xvector(const float* iData)
{
	return _mm_load_ps(iData);
}

//////////////////////////////////////////////////////////////////////////

inline XVector xvector(float iX, float iY, float iZ, float iW)
{
	return _mm_set_ps(iW, iZ, iY, iX);
}

//////////////////////////////////////////////////////////////////////////

inline XVector xvector(float iScalar)
{
	return _mm_set_ps1(iScalar);
}

//////////////////////////////////////////////////////////////////////////
// setters
inline XVector setX(XVectorParam iVec, float iX)
{
	XVectorConvert access;
	access.v = iVec;
	access.f[0] = iX;
	return access.v;
}

//////////////////////////////////////////////////////////////////////////

inline XVector setY(XVectorParam iVec, float iY)
{
	XVectorConvert access;
	access.v = iVec;
	access.f[1] = iY;
	return access.v;
}

//////////////////////////////////////////////////////////////////////////

inline XVector setZ(XVectorParam iVec, float iZ)
{
	XVectorConvert access;
	access.v = iVec;
	access.f[2] = iZ;
	return access.v;
}

//////////////////////////////////////////////////////////////////////////

inline XVector setW(XVectorParam iVec, float iW)
{
	XVectorConvert access;
	access.v = iVec;
	access.f[3] = iW;
	return access.v;
}

//////////////////////////////////////////////////////////////////////////
// getters

inline float getX(XVectorParam iVec)
{
	XVectorConvert access;
	access.v = iVec;
	return access.f[0];
}

//////////////////////////////////////////////////////////////////////////

inline float getY(XVectorParam iVec)
{
	XVectorConvert access;
	access.v = iVec;
	return access.f[1];
}

//////////////////////////////////////////////////////////////////////////

inline float getZ(XVectorParam iVec)
{
	XVectorConvert access;
	access.v = iVec;
	return access.f[2];
}

//////////////////////////////////////////////////////////////////////////

inline float getW(XVectorParam iVec)
{
	XVectorConvert access;
	access.v = iVec;
	return access.f[3];
}

//////////////////////////////////////////////////////////////////////////

inline float   getComponent(XVectorParam iVec, int iIndex)
{
	XVectorConvert access;
	access.v = iVec;
	return access.f[iIndex];
}

//////////////////////////////////////////////////////////////////////////
// Extended vector arithmetic

inline float dot(XVectorParam iVecA, XVectorParam iVecB)
{
	XVector ret = dotVector(iVecA, iVecB);
	return getX(ret);
}

//////////////////////////////////////////////////////////////////////////

inline float distanceSq(XVectorParam iVecA, XVectorParam iVecB)
{
	XVector ret = dotVector(iVecA, iVecB);
	return getX(ret);
}

//////////////////////////////////////////////////////////////////////////

inline float lengthSq(XVectorParam iVec)
{
	XVector ret = dotVector(iVec,iVec);
	return getX(ret);
}

//////////////////////////////////////////////////////////////////////////

inline float length(XVectorParam iVec)
{
	XVector lengthSq = dotVector(iVec,iVec);
	return getX(_mm_sqrt_ps(lengthSq));
}

//////////////////////////////////////////////////////////////////////////

inline float distance(XVectorParam iVecA, XVectorParam iVecB)
{
	XVector lengthSq = dotVector(iVecA,iVecB);
	return getX(_mm_sqrt_ps(lengthSq));
}

//////////////////////////////////////////////////////////////////////////

inline XVector dotVector(XVectorParam iVecA, XVectorParam iVecB)
{
	XVector ret = _mm_mul_ps(iVecA, iVecB);
	// (x + y) + (z + w)
	// x y z w       x y z w
	// + + + +  ==>  + + + +
	// x y z w       y x w z
	XVector tmp1 = _mm_shuffle_ps(ret, ret, EM_SHUFFLE(1,0,3,2));
	XVector tmp2 = _mm_add_ps(tmp1, ret);
	// xy xy zw zw
	//  +  +  +  +
	// zw zw xy xy
	tmp1 = _mm_shuffle_ps(tmp2, tmp2, EM_SHUFFLE(3,3,0,0));
	ret = _mm_add_ps(tmp1, tmp2);
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline XVector distanceSqVector(XVectorParam iVecA, XVectorParam iVecB)
{
	return dotVector(iVecA, iVecB);    
}

//////////////////////////////////////////////////////////////////////////

inline XVector lengthSqVector(XVectorParam iVec)
{
	return dotVector(iVec,iVec);
}

//////////////////////////////////////////////////////////////////////////

inline XVector lengthVector(XVectorParam iVec)
{
	XVector lengthSq = dotVector(iVec,iVec);
	return _mm_sqrt_ps(lengthSq);
}

//////////////////////////////////////////////////////////////////////////

inline XVector distanceVector(XVectorParam iVecA, XVectorParam iVecB)
{
	XVector lengthSq = dotVector(iVecA,iVecB);
	return _mm_sqrt_ps(lengthSq);
}

//////////////////////////////////////////////////////////////////////////

inline XVector normalize(XVectorParam iVec)
{
	XVector lengthSq = dotVector(iVec, iVec);
	XVector recipSq = _mm_rsqrt_ps(lengthSq);
	return _mm_mul_ps(iVec, recipSq);
}

//////////////////////////////////////////////////////////////////////////

inline XVector cross(XVectorParam iVecA, XVectorParam iVecB)
{
	// Assumes w component is zero

	// Cross product
	// Rx = (iVecA.y * iVecB.z - iVecA.z * iVecB.y)
	// Ry = (iVecA.z * iVecB.x - iVecA.x * iVecB.z)
	// Rz = (iVecA.x * iVecB.y - iVecA.y * iVecB.x)
	// Rw = (iVecA.w * iVecB.w - iVecA.w * iVecB.w) = 0

	// iVecA (x, y, z, w)
	// iVecB (y, z, x, w)

	XVector shuffle = _mm_shuffle_ps(iVecB, iVecB, EM_SHUFFLE(1,2,0,3));
	XVector leftColumn = _mm_mul_ps(iVecA, shuffle);

	// iVecB (x, y, z, w)
	// iVecA (y, z, x, w)
	shuffle = _mm_shuffle_ps(iVecA, iVecA, EM_SHUFFLE(1,2,0,3));
	XVector rightColumn = _mm_mul_ps(iVecB, shuffle);

	XVector result = _mm_sub_ps(leftColumn, rightColumn);

	// actual res (y, z, x, w)
	// wanted out (x, y, z, w)
	return _mm_shuffle_ps(result, result, EM_SHUFFLE(1,2,0,3));  
}

//////////////////////////////////////////////////////////////////////////

inline float sqrt(XVectorParam iVec)
{
	return getX(_mm_sqrt_ps(iVec));
}

//////////////////////////////////////////////////////////////////////////

inline float rsqrt(XVectorParam iVec)
{
	return getX(_mm_rsqrt_ps(iVec));
}

//////////////////////////////////////////////////////////////////////////

inline XVector sqrtVector(XVectorParam iVec)
{
	return _mm_sqrt_ps(iVec);
}

//////////////////////////////////////////////////////////////////////////

inline XVector rsqrtVector(XVectorParam iVec)
{
	return _mm_rsqrt_ps(iVec);
}

//////////////////////////////////////////////////////////////////////////
// Utility

inline XVector splatX(XVectorParam iVec)
{
	return _mm_shuffle_ps(iVec,iVec, EM_SHUFFLE(0,0,0,0));
}

//////////////////////////////////////////////////////////////////////////

inline XVector splatY(XVectorParam iVec)
{
	return _mm_shuffle_ps(iVec,iVec, EM_SHUFFLE(1,1,1,1));
}

//////////////////////////////////////////////////////////////////////////

inline XVector splatZ(XVectorParam iVec)
{
	return _mm_shuffle_ps(iVec,iVec, EM_SHUFFLE(2,2,2,2));
}

//////////////////////////////////////////////////////////////////////////

inline XVector splatW(XVectorParam iVec)
{
	return _mm_shuffle_ps(iVec,iVec, EM_SHUFFLE(3,3,3,3));
}

