
//////////////////////////////////////////////////////////////////////////
// Basic vector arithmetic

inline Vector2 negate(Vector2Param iVec)
{
	Vector2 ret;
	ret.x = -iVec.x;
	ret.y = -iVec.y;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector2 add(Vector2Param iVecA, Vector2Param iVecB)
{
	Vector2 ret;
	ret.x = iVecA.x + iVecB.x;
	ret.y = iVecA.y + iVecB.y;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector2 sub(Vector2Param iVecA, Vector2Param iVecB)
{
	Vector2 ret;
	ret.x = iVecA.x - iVecB.x;
	ret.y = iVecA.y - iVecB.y;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector2 mul(Vector2Param iVecA, Vector2Param iVecB)
{
	Vector2 ret;
	ret.x = iVecA.x * iVecB.x;
	ret.y = iVecA.y * iVecB.y;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector2 div(Vector2Param iVecA, Vector2Param iVecB)
{
	Vector2 ret;
	ret.x = iVecA.x / iVecB.x;
	ret.y = iVecA.y / iVecB.y;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline bool aproxEqual(Vector2Param iVecA, Vector2Param iVecB)
{
	return ( (iVecA.x <= iVecB.x + Math::kFLOAT_EPS && iVecA.x >= iVecB.x - Math::kFLOAT_EPS) &&
		(iVecA.y <= iVecB.y + Math::kFLOAT_EPS && iVecA.y >= iVecB.y - Math::kFLOAT_EPS) );
}

//////////////////////////////////////////////////////////////////////////

inline bool equal(Vector2Param iVecA, Vector2Param iVecB)
{
	return ( iVecA.x == iVecB.x && iVecA.y == iVecB.y );
}


//////////////////////////////////////////////////////////////////////////
// operators

inline bool operator==(Vector2Param iVecA, Vector2Param iVecB)
{
	return equal(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline bool operator!=(Vector2Param iVecA, Vector2Param iVecB)
{
	return !equal(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline Vector2 operator-(Vector2Param iVec)
{
	return negate(iVec);
}

//////////////////////////////////////////////////////////////////////////

inline Vector2 operator+(Vector2Param iVecA, Vector2Param iVecB)
{
	return add(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline Vector2 operator-(Vector2Param iVecA, Vector2Param iVecB)
{
	return sub(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline Vector2 operator*(Vector2Param iVecA, Vector2Param iVecB)
{
	return mul(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline Vector2 operator/(Vector2Param iVecA, Vector2Param iVecB)
{
	return div(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline Vector2& operator+=(Vector2& ioVecA, Vector2Param iVecB)
{
	ioVecA = add(ioVecA, iVecB);
	return ioVecA;
}

//////////////////////////////////////////////////////////////////////////

inline Vector2& operator-=(Vector2& ioVecA, Vector2Param iVecB)
{
	ioVecA = sub(ioVecA, iVecB);
	return ioVecA;
}

//////////////////////////////////////////////////////////////////////////

inline Vector2& operator*=(Vector2& ioVecA, Vector2Param iVecB)
{
	ioVecA = mul(ioVecA, iVecB);
	return ioVecA;
}

//////////////////////////////////////////////////////////////////////////

inline Vector2& operator/=(Vector2& ioVecA, Vector2Param iVecB)
{
	ioVecA = div(ioVecA, iVecB);
	return ioVecA;
}

//////////////////////////////////////////////////////////////////////////

inline Vector2 operator*(Vector2Param iVec, float iScalar)
{
	Vector2 ret = iVec;
	ret.x *= iScalar;
	ret.y *= iScalar;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector2 operator/(Vector2Param iVec, float iScalar)
{
	Vector2 ret = iVec;
	ret.x /= iScalar;
	ret.y /= iScalar;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector2& operator*=(Vector2& ioVec, float iScalar)
{
	ioVec.x *= iScalar;
	ioVec.y *= iScalar;
	return ioVec;
}

//////////////////////////////////////////////////////////////////////////

inline Vector2& operator/=(Vector2& ioVec, float iScalar)
{
	ioVec.x /= iScalar;
	ioVec.y /= iScalar;
	return ioVec;
}

//////////////////////////////////////////////////////////////////////////
// factory

inline Vector2 vector2(const float* iData)
{
	Vector2 ret = { iData[0], iData[1] };
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector2 vector2(float iX, float iY)
{
	Vector2 ret = { iX, iY };
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector2 vector2(float iScalar)
{
	Vector2 ret = { iScalar, iScalar };
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// getters

inline float   getComponent(Vector2Param iVec, int iIndex)
{
	return iVec.v[iIndex];
}

//////////////////////////////////////////////////////////////////////////
// extended vector arithmetic

inline float dot(Vector2Param iVecA, Vector2Param iVecB)
{
	return iVecA.x*iVecB.x+iVecA.y*iVecB.y;
}

//////////////////////////////////////////////////////////////////////////

inline float distanceSq(Vector2Param iVecA, Vector2Param iVecB)
{
	return dot(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline float lengthSq(Vector2Param iVec)
{
	return dot(iVec,iVec);
}

//////////////////////////////////////////////////////////////////////////

inline float length(Vector2Param iVec)
{
	return sqrtf(iVec.x*iVec.x+iVec.y*iVec.y);
}

inline float distance(Vector2Param iVecA, Vector2Param iVecB)
{
	return sqrtf(iVecA.x*iVecB.x+iVecA.y*iVecB.y);
}

//////////////////////////////////////////////////////////////////////////

inline Vector2 normalize(Vector2Param iVec)
{
	float recipSq = 1.f/sqrtf(iVec.x*iVec.x+iVec.y*iVec.y);
	Vector2 ret = iVec * recipSq;
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// utility

inline Vector2 splatX(Vector2Param iVec)
{
	Vector2 ret = {iVec.x, iVec.x};
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector2 splatY(Vector2Param iVec)
{
	Vector2 ret = {iVec.y, iVec.y};
	return ret;
}
