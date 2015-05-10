
//////////////////////////////////////////////////////////////////////////
// basic vector arithmetic

inline XVector negate(XVectorParam iVec)
{
	XVector ret;
	ret.x = -iVec.x;
	ret.y = -iVec.y;
	ret.z = -iVec.z;
	ret.w = -iVec.w;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline XVector add(XVectorParam iVecA, XVectorParam iVecB)
{
	XVector ret;
	ret.x = iVecA.x + iVecB.x;
	ret.y = iVecA.y + iVecB.y;
	ret.z = iVecA.z + iVecB.z;
	ret.w = iVecA.w + iVecB.w;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline XVector sub(XVectorParam iVecA, XVectorParam iVecB)
{
	XVector ret;
	ret.x = iVecA.x - iVecB.x;
	ret.y = iVecA.y - iVecB.y;
	ret.z = iVecA.z - iVecB.z;
	ret.w = iVecA.w - iVecB.w;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline XVector mul(XVectorParam iVecA, XVectorParam iVecB)
{
	XVector ret;
	ret.x = iVecA.x * iVecB.x;
	ret.y = iVecA.y * iVecB.y;
	ret.z = iVecA.z * iVecB.z;
	ret.w = iVecA.w * iVecB.w;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline XVector div(XVectorParam iVecA, XVectorParam iVecB)
{
	XVector ret;
	ret.x = iVecA.x / iVecB.x;
	ret.y = iVecA.y / iVecB.y;
	ret.z = iVecA.z / iVecB.z;
	ret.w = iVecA.w / iVecB.w;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline bool aproxEqual(XVectorParam iVecA, XVectorParam iVecB)
{
	return ( (iVecA.x <= iVecB.x + Math::kFLOAT_EPS && iVecA.x >= iVecB.x - Math::kFLOAT_EPS) &&
		(iVecA.y <= iVecB.y + Math::kFLOAT_EPS && iVecA.y >= iVecB.y - Math::kFLOAT_EPS) &&
		(iVecA.z <= iVecB.z + Math::kFLOAT_EPS && iVecA.z >= iVecB.z - Math::kFLOAT_EPS) &&
		(iVecA.w <= iVecB.w + Math::kFLOAT_EPS && iVecA.w >= iVecB.w - Math::kFLOAT_EPS) );
}

//////////////////////////////////////////////////////////////////////////

inline bool equal(XVectorParam iVecA, XVectorParam iVecB)
{
	return ( iVecA.x == iVecB.x && iVecA.y == iVecB.y && iVecA.z == iVecB.z && iVecA.w == iVecB.w );
}

//////////////////////////////////////////////////////////////////////////
// operators

inline bool operator==(XVectorParam iVecA, XVectorParam iVecB)
{
	return equal(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline bool operator!=(XVectorParam iVecA, XVectorParam iVecB)
{
	return !equal(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

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

inline XVector& operator+=(XVector& ioVecA, XVectorParam iVecB)
{
	ioVecA = add(ioVecA, iVecB);
	return ioVecA;
}

//////////////////////////////////////////////////////////////////////////

inline XVector& operator-=(XVector& ioVecA, XVectorParam iVecB)
{
	ioVecA = sub(ioVecA, iVecB);
	return ioVecA;
}

//////////////////////////////////////////////////////////////////////////

inline XVector& operator*=(XVector& ioVecA, XVectorParam iVecB)
{
	ioVecA = mul(ioVecA, iVecB);
	return ioVecA;
}

//////////////////////////////////////////////////////////////////////////

inline XVector& operator/=(XVector& ioVecA, XVectorParam iVecB)
{
	ioVecA = div(ioVecA, iVecB);
	return ioVecA;
}

//////////////////////////////////////////////////////////////////////////

inline XVector operator*(XVectorParam iVec, float iScalar)
{
	XVector ret = iVec;
	ret.x *= iScalar;
	ret.y *= iScalar;
	ret.z *= iScalar;
	ret.w *= iScalar;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline XVector operator/(XVectorParam iVec, float iScalar)
{
	XVector ret = iVec;
	ret.x /= iScalar;
	ret.y /= iScalar;
	ret.z /= iScalar;
	ret.w /= iScalar;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline XVector& operator*=(XVector& ioVec, float iScalar)
{
	ioVec.x *= iScalar;
	ioVec.y *= iScalar;
	ioVec.z *= iScalar;
	ioVec.w *= iScalar;
	return ioVec;
}

//////////////////////////////////////////////////////////////////////////

inline XVector& operator/=(XVector& ioVec, float iScalar)
{
	ioVec.x /= iScalar;
	ioVec.y /= iScalar;
	ioVec.z /= iScalar;
	ioVec.w /= iScalar;
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

inline XVector xvector( const float* iData )
{
	XVector ret = { iData[0], iData[1], iData[2], iData[3] };
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline XVector xvector(float iX, float iY, float iZ, float iW )
{
	XVector ret = {iX,iY,iZ,iW};
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline XVector xvector(float iScalar)
{
	XVector ret = {iScalar, iScalar, iScalar, iScalar};
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// setters
inline XVector setX(XVectorParam iVec, float iX)
{
	XVector ret = iVec;
	ret.v[0] = iX;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline XVector setY(XVectorParam iVec, float iY)
{
	XVector ret = iVec;
	ret.v[1] = iY;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline XVector setZ(XVectorParam iVec, float iZ)
{
	XVector ret = iVec;
	ret.v[2] = iZ;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline XVector setW(XVectorParam iVec, float iW)
{
	XVector ret = iVec;
	ret.v[3] = iW;
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// getters

inline float getX(XVectorParam iVec)
{
	return iVec.v[0];
}

//////////////////////////////////////////////////////////////////////////

inline float getY(XVectorParam iVec)
{
	return iVec.v[1];
}

//////////////////////////////////////////////////////////////////////////

inline float getZ(XVectorParam iVec)
{
	return iVec.v[2];
}

//////////////////////////////////////////////////////////////////////////

inline float getW(XVectorParam iVec)
{
	return iVec.v[3];
}

//////////////////////////////////////////////////////////////////////////

inline float getComponent(XVectorParam iVec, int iIndex)
{
	return iVec.v[iIndex];
}

//////////////////////////////////////////////////////////////////////////
// extended vector arithmetic

inline float dot(XVectorParam iVecA, XVectorParam iVecB)
{
	return iVecA.x*iVecB.x+iVecA.y*iVecB.y+iVecA.z*iVecB.z+iVecA.w*iVecB.w;
}

//////////////////////////////////////////////////////////////////////////

inline float distanceSq(XVectorParam iVecA, XVectorParam iVecB)
{
	return dot(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline float lengthSq(XVectorParam iVec)
{
	return dot(iVec,iVec);
}

//////////////////////////////////////////////////////////////////////////

inline float length(XVectorParam iVec)
{
	return sqrtf(iVec.x*iVec.x+iVec.y*iVec.y+iVec.z*iVec.z+iVec.w*iVec.w);
}

inline float distance(XVectorParam iVecA, XVectorParam iVecB)
{
	return sqrtf(iVecA.x*iVecB.x+iVecA.y*iVecB.y+iVecA.z*iVecB.z+iVecA.w*iVecB.w);
}

//////////////////////////////////////////////////////////////////////////

inline XVector dotVector(XVectorParam iVecA, XVectorParam iVecB)
{
	return xvector(dot(iVecA, iVecB));
}

//////////////////////////////////////////////////////////////////////////

inline XVector distanceSqVector(XVectorParam iVecA, XVectorParam iVecB)
{
	return xvector(distanceSq(iVecA, iVecB));
}

//////////////////////////////////////////////////////////////////////////

inline XVector lengthSqVector(XVectorParam iVec)
{
	return xvector(lengthSq(iVec));
}

//////////////////////////////////////////////////////////////////////////

inline XVector lengthVector(XVectorParam iVec)
{
	return xvector(length(iVec));
}

//////////////////////////////////////////////////////////////////////////

inline XVector distanceVector(XVectorParam iVecA, XVectorParam iVecB)
{
	return xvector(distance(iVecA, iVecB));
}

//////////////////////////////////////////////////////////////////////////

inline XVector normalize(XVectorParam iVec)
{
	float recipSq = 1.f/sqrtf( iVec.x*iVec.x+iVec.y*iVec.y+iVec.z*iVec.z+iVec.w*iVec.w);
	XVector ret = iVec;
	ret.x *= recipSq;
	ret.y *= recipSq;
	ret.z *= recipSq;
	ret.w *= recipSq;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline XVector cross(XVectorParam iVecA, XVectorParam iVecB)
{
	XVector ret;
	ret.x = iVecA.y * iVecB.z - iVecA.z * iVecB.y;
	ret.y = iVecA.z * iVecB.x - iVecA.x * iVecB.z;
	ret.z = iVecA.x * iVecB.y - iVecA.y * iVecB.x;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline float sqrt(XVectorParam iVec)
{
	return sqrtf(iVec.x);
}

//////////////////////////////////////////////////////////////////////////

inline float rsqrt(XVectorParam iVec)
{
	return 1.f/sqrtf(iVec.x);
}

//////////////////////////////////////////////////////////////////////////

inline XVector sqrtVector(XVectorParam iVec)
{
	return xvector(sqrtf(iVec.x),sqrtf(iVec.y),sqrtf(iVec.z),sqrtf(iVec.w));
}

//////////////////////////////////////////////////////////////////////////

inline XVector rsqrtVector(XVectorParam iVec)
{
	return xvector(1.f/sqrtf(iVec.x),1.f/sqrtf(iVec.y),1.f/sqrtf(iVec.z),1.f/sqrtf(iVec.w));
}

//////////////////////////////////////////////////////////////////////////
// utility

inline XVector splatX(XVectorParam iVec)
{
	XVector ret = {iVec.x, iVec.x, iVec.x, iVec.x};
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline XVector splatY(XVectorParam iVec)
{
	XVector ret = {iVec.y, iVec.y, iVec.y, iVec.y};
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline XVector splatZ(XVectorParam iVec)
{
	XVector ret = {iVec.z, iVec.z, iVec.z, iVec.z};
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline XVector splatW(XVectorParam iVec)
{
	XVector ret = {iVec.w, iVec.w, iVec.w, iVec.w};
	return ret;
}

