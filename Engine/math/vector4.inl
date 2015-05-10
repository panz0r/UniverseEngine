
//////////////////////////////////////////////////////////////////////////
// basic vector arithmetic

inline Vector4 negate(Vector4Param iVec)
{
	Vector4 ret;
	ret.x = -iVec.x;
	ret.y = -iVec.y;
	ret.z = -iVec.z;
	ret.w = -iVec.w;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 add(Vector4Param iVecA, Vector4Param iVecB)
{
	Vector4 ret;
	ret.x = iVecA.x + iVecB.x;
	ret.y = iVecA.y + iVecB.y;
	ret.z = iVecA.z + iVecB.z;
	ret.w = iVecA.w + iVecB.w;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 sub(Vector4Param iVecA, Vector4Param iVecB)
{
	Vector4 ret;
	ret.x = iVecA.x - iVecB.x;
	ret.y = iVecA.y - iVecB.y;
	ret.z = iVecA.z - iVecB.z;
	ret.w = iVecA.w - iVecB.w;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 mul(Vector4Param iVecA, Vector4Param iVecB)
{
	Vector4 ret;
	ret.x = iVecA.x * iVecB.x;
	ret.y = iVecA.y * iVecB.y;
	ret.z = iVecA.z * iVecB.z;
	ret.w = iVecA.w * iVecB.w;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 div(Vector4Param iVecA, Vector4Param iVecB)
{
	Vector4 ret;
	ret.x = iVecA.x / iVecB.x;
	ret.y = iVecA.y / iVecB.y;
	ret.z = iVecA.z / iVecB.z;
	ret.w = iVecA.w / iVecB.w;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline bool aproxEqual(Vector4Param iVecA, Vector4Param iVecB)
{
	return ( (iVecA.x <= iVecB.x + Math::kFLOAT_EPS && iVecA.x >= iVecB.x - Math::kFLOAT_EPS) &&
		(iVecA.y <= iVecB.y + Math::kFLOAT_EPS && iVecA.y >= iVecB.y - Math::kFLOAT_EPS) &&
		(iVecA.z <= iVecB.z + Math::kFLOAT_EPS && iVecA.z >= iVecB.z - Math::kFLOAT_EPS) &&
		(iVecA.w <= iVecB.w + Math::kFLOAT_EPS && iVecA.w >= iVecB.w - Math::kFLOAT_EPS) );
}

//////////////////////////////////////////////////////////////////////////

inline bool equal(Vector4Param iVecA, Vector4Param iVecB)
{
	return ( iVecA.x == iVecB.x && iVecA.y == iVecB.y && iVecA.z == iVecB.z && iVecA.w == iVecB.w );
}

//////////////////////////////////////////////////////////////////////////
// operators

inline bool operator==(Vector4Param iVecA, Vector4Param iVecB)
{
	return equal(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline bool operator!=(Vector4Param iVecA, Vector4Param iVecB)
{
	return !equal(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 operator-(Vector4Param iVec)
{
	return negate(iVec);
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 operator+(Vector4Param iVecA, Vector4Param iVecB)
{
	return add(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 operator-(Vector4Param iVecA, Vector4Param iVecB)
{
	return sub(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 operator*(Vector4Param iVecA, Vector4Param iVecB)
{
	return mul(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 operator/(Vector4Param iVecA, Vector4Param iVecB)
{
	return div(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline Vector4& operator+=(Vector4& ioVecA, Vector4Param iVecB)
{
	ioVecA = add(ioVecA, iVecB);
	return ioVecA;
}

//////////////////////////////////////////////////////////////////////////

inline Vector4& operator-=(Vector4& ioVecA, Vector4Param iVecB)
{
	ioVecA = sub(ioVecA, iVecB);
	return ioVecA;
}

//////////////////////////////////////////////////////////////////////////

inline Vector4& operator*=(Vector4& ioVecA, Vector4Param iVecB)
{
	ioVecA = mul(ioVecA, iVecB);
	return ioVecA;
}

//////////////////////////////////////////////////////////////////////////

inline Vector4& operator/=(Vector4& ioVecA, Vector4Param iVecB)
{
	ioVecA = div(ioVecA, iVecB);
	return ioVecA;
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 operator*(Vector4Param iVec, float iScalar)
{
	Vector4 ret = iVec;
	ret.x *= iScalar;
	ret.y *= iScalar;
	ret.z *= iScalar;
	ret.w *= iScalar;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 operator/(Vector4Param iVec, float iScalar)
{
	Vector4 ret = iVec;
	ret.x /= iScalar;
	ret.y /= iScalar;
	ret.z /= iScalar;
	ret.w /= iScalar;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector4& operator*=(Vector4& ioVec, float iScalar)
{
	ioVec.x *= iScalar;
	ioVec.y *= iScalar;
	ioVec.z *= iScalar;
	ioVec.w *= iScalar;
	return ioVec;
}

//////////////////////////////////////////////////////////////////////////

inline Vector4& operator/=(Vector4& ioVec, float iScalar)
{
	ioVec.x /= iScalar;
	ioVec.y /= iScalar;
	ioVec.z /= iScalar;
	ioVec.w /= iScalar;
	return ioVec;
}

//////////////////////////////////////////////////////////////////////////
// factory

inline Vector4 vector4( const float* iData )
{
	Vector4 ret = { iData[0], iData[1], iData[2], iData[3] };
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 vector4(float iX, float iY, float iZ, float iW )
{
	Vector4 ret = {iX,iY,iZ,iW};
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 vector4(float iScalar)
{
	Vector4 ret = {iScalar, iScalar, iScalar, iScalar};
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 vector4(const Vector3& iVec)
{
	return vector4(iVec.x, iVec.y, iVec.z, 0.f);
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 vector4(const Vector3& iVec, float iW)
{
	return vector4(iVec.x, iVec.y, iVec.z, iW);
}

//////////////////////////////////////////////////////////////////////////
// getters

inline float   getComponent(Vector4Param iVec, int iIndex)
{
	return iVec.v[iIndex];
}

//////////////////////////////////////////////////////////////////////////
// extended vector arithmetic

inline float dot(Vector4Param iVecA, Vector4Param iVecB)
{
	return iVecA.x*iVecB.x+iVecA.y*iVecB.y+iVecA.z*iVecB.z+iVecA.w*iVecB.w;
}

//////////////////////////////////////////////////////////////////////////

inline float distanceSq(Vector4Param iVecA, Vector4Param iVecB)
{
	return dot(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline float lengthSq(Vector4Param iVec)
{
	return dot(iVec,iVec);
}

//////////////////////////////////////////////////////////////////////////

inline float length(Vector4Param iVec)
{
	return sqrtf(iVec.x*iVec.x+iVec.y*iVec.y+iVec.z*iVec.z+iVec.w*iVec.w);
}

inline float distance(Vector4Param iVecA, Vector4Param iVecB)
{
	return sqrtf(iVecA.x*iVecB.x+iVecA.y*iVecB.y+iVecA.z*iVecB.z+iVecA.w*iVecB.w);
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 normalize(Vector4Param iVec)
{
	float recipSq = 1.f/sqrtf( iVec.x*iVec.x+iVec.y*iVec.y+iVec.z*iVec.z+iVec.w*iVec.w);
	Vector4 ret = iVec;
	ret.x *= recipSq;
	ret.y *= recipSq;
	ret.z *= recipSq;
	ret.w *= recipSq;
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// utility

inline Vector4 splatX(Vector4Param iVec)
{
	Vector4 ret = {iVec.x, iVec.x, iVec.x, iVec.x};
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 splatY(Vector4Param iVec)
{
	Vector4 ret = {iVec.y, iVec.y, iVec.y, iVec.y};
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 splatZ(Vector4Param iVec)
{
	Vector4 ret = {iVec.z, iVec.z, iVec.z, iVec.z};
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 splatW(Vector4Param iVec)
{
	Vector4 ret = {iVec.w, iVec.w, iVec.w, iVec.w};
	return ret;
}

//////////////////////////////////////////////////////////////////////////

template <>
inline Vector4 Math::saturate<Vector4>(Vector4 iVec)
{
	Vector4 ret;
	ret.x = clamp(iVec.x, 0.f, 1.f);
	ret.y = clamp(iVec.y, 0.f, 1.f);
	ret.z = clamp(iVec.z, 0.f, 1.f);
	ret.w = clamp(iVec.w, 0.f, 1.f);
	return ret;
}