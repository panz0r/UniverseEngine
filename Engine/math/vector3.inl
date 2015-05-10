
//////////////////////////////////////////////////////////////////////////
// basic vector arithmetic

inline Vector3 negate(Vector3Param iVec)
{
	Vector3 ret;
	ret.x = -iVec.x;
	ret.y = -iVec.y;
	ret.z = -iVec.z;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 add(Vector3Param iVecA, Vector3Param iVecB)
{
	Vector3 ret;
	ret.x = iVecA.x + iVecB.x;
	ret.y = iVecA.y + iVecB.y;
	ret.z = iVecA.z + iVecB.z;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 sub(Vector3Param iVecA, Vector3Param iVecB)
{
	Vector3 ret;
	ret.x = iVecA.x - iVecB.x;
	ret.y = iVecA.y - iVecB.y;
	ret.z = iVecA.z - iVecB.z;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 mul(Vector3Param iVecA, Vector3Param iVecB)
{
	Vector3 ret;
	ret.x = iVecA.x * iVecB.x;
	ret.y = iVecA.y * iVecB.y;
	ret.z = iVecA.z * iVecB.z;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 div(Vector3Param iVecA, Vector3Param iVecB)
{
	Vector3 ret;
	ret.x = iVecA.x / iVecB.x;
	ret.y = iVecA.y / iVecB.y;
	ret.z = iVecA.z / iVecB.z;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 mul(Vector3Param iVecA, float iScalar)
{
	Vector3 ret;
	ret.x = iVecA.x * iScalar;
	ret.y = iVecA.y * iScalar;
	ret.z = iVecA.z * iScalar;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 div(Vector3Param iVecA, float iScalar)
{
	Vector3 ret;
	ret.x = iVecA.x / iScalar;
	ret.y = iVecA.y / iScalar;
	ret.z = iVecA.z / iScalar;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline bool aproxEqual(Vector3Param iVecA, Vector3Param iVecB, float iEpsilon /* = Math::kFLOAT_EPS */)
{
	return ( (iVecA.x <= iVecB.x + iEpsilon && iVecA.x >= iVecB.x - iEpsilon) &&
		(iVecA.y <= iVecB.y + iEpsilon && iVecA.y >= iVecB.y - iEpsilon) &&
		(iVecA.z <= iVecB.z +iEpsilon && iVecA.z >= iVecB.z - iEpsilon) );
}

//////////////////////////////////////////////////////////////////////////

inline bool equal(Vector3Param iVecA, Vector3Param iVecB)
{
	return ( iVecA.x == iVecB.x && iVecA.y == iVecB.y && iVecA.z == iVecB.z );
}


//////////////////////////////////////////////////////////////////////////
// operators

inline bool operator==(Vector3Param iVecA, Vector3Param iVecB)
{
	return equal(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline bool operator!=(Vector3Param iVecA, Vector3Param iVecB)
{
	return !equal(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 operator-(Vector3Param iVec)
{
	return negate(iVec);
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 operator+(Vector3Param iVecA, Vector3Param iVecB)
{
	return add(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 operator-(Vector3Param iVecA, Vector3Param iVecB)
{
	return sub(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 operator*(Vector3Param iVecA, Vector3Param iVecB)
{
	return mul(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 operator/(Vector3Param iVecA, Vector3Param iVecB)
{
	return div(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline Vector3& operator+=(Vector3& ioVecA, Vector3Param iVecB)
{
	ioVecA = add(ioVecA, iVecB);
	return ioVecA;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3& operator-=(Vector3& ioVecA, Vector3Param iVecB)
{
	ioVecA = sub(ioVecA, iVecB);
	return ioVecA;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3& operator*=(Vector3& ioVecA, Vector3Param iVecB)
{
	ioVecA = mul(ioVecA, iVecB);
	return ioVecA;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3& operator/=(Vector3& ioVecA, Vector3Param iVecB)
{
	ioVecA = div(ioVecA, iVecB);
	return ioVecA;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 operator*(Vector3Param iVec, float iScalar)
{
	Vector3 ret = iVec;
	ret.x *= iScalar;
	ret.y *= iScalar;
	ret.z *= iScalar;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 operator/(Vector3Param iVec, float iScalar)
{
	Vector3 ret = iVec;
	ret.x /= iScalar;
	ret.y /= iScalar;
	ret.z /= iScalar;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 operator*(float iScalar, Vector3Param iVec)
{
	Vector3 ret = iVec;
	ret.x *= iScalar;
	ret.y *= iScalar;
	ret.z *= iScalar;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 operator/(float iScalar, Vector3Param iVec)
{
	Vector3 ret = iVec;
	ret.x /= iScalar;
	ret.y /= iScalar;
	ret.z /= iScalar;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3& operator*=(Vector3& ioVec, float iScalar)
{
	ioVec.x *= iScalar;
	ioVec.y *= iScalar;
	ioVec.z *= iScalar;
	return ioVec;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3& operator/=(Vector3& ioVec, float iScalar)
{
	ioVec.x /= iScalar;
	ioVec.y /= iScalar;
	ioVec.z /= iScalar;
	return ioVec;
}


//////////////////////////////////////////////////////////////////////////
// factory

inline Vector3 vector3(Vector2Param iVec)
{
	return vector3(iVec.x, iVec.y, 0.f);
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 vector3(Vector4Param iVec)
{
	return vector3(iVec.x, iVec.y, iVec.z);
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 vector3(const float* iData)
{
	Vector3 ret = { iData[0], iData[1], iData[2] };
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 vector3(float iX, float iY, float iZ)
{
	Vector3 ret = { iX, iY, iZ };
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 vector3(float iScalar)
{
	Vector3 ret = { iScalar, iScalar, iScalar };
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// Getters

inline float   getComponent(Vector3Param iVec, int iIndex)
{
	return iVec.v[iIndex];
}

//////////////////////////////////////////////////////////////////////////
// Extended vector arithmetic

inline float dot(Vector3Param iVecA, Vector3Param iVecB)
{
	return iVecA.x*iVecB.x+iVecA.y*iVecB.y+iVecA.z*iVecB.z;
}

//////////////////////////////////////////////////////////////////////////

inline float distanceSq(Vector3Param iVecA, Vector3Param iVecB)
{
	return dot(iVecA, iVecB);
}

//////////////////////////////////////////////////////////////////////////

inline float lengthSq(Vector3Param iVec)
{
	return dot(iVec,iVec);
}

//////////////////////////////////////////////////////////////////////////

inline float length(Vector3Param iVec)
{
	return sqrtf(iVec.x*iVec.x+iVec.y*iVec.y+iVec.z*iVec.z);
}

//////////////////////////////////////////////////////////////////////////

inline float distance(Vector3Param iVecA, Vector3Param iVecB)
{
	Vector3 distanceVec = iVecA - iVecB;
	return length(distanceVec);
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 normalize(Vector3Param iVec)
{
	float recipSq = 1.f/sqrtf(dot(iVec, iVec));
	return mul(iVec, recipSq);
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 normalize(Vector3Param iVec, float& oLength)
{
	oLength = sqrtf(dot(iVec, iVec));
	float recipSq = 1.f/oLength;
	return mul(iVec, recipSq);
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 cross(Vector3Param iVecA, Vector3Param iVecB)
{
	Vector3 ret;
	ret.x = iVecA.y * iVecB.z - iVecA.z * iVecB.y;
	ret.y = iVecA.z * iVecB.x - iVecA.x * iVecB.z;
	ret.z = iVecA.x * iVecB.y - iVecA.y * iVecB.x;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 reflect(Vector3Param iVec, Vector3Param iNormal)
{
	Vector3 ret;
	ret = iVec - 2.f * dot(iVec, iNormal) * iNormal;
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 refract(Vector3Param iVec, Vector3Param iNormal, float iIndexOfRefraction)
{
	bool inside = dot(iVec, iNormal) > 0.f;
	Vector3 normal = (inside) ? -iNormal : iNormal;
	float eta = (inside) ? iIndexOfRefraction : 1.f / iIndexOfRefraction;
	float cosi = dot(normal, iVec);
	float k = 1.f - eta*eta * (1.f - cosi*cosi);
	Vector3 ret;
	ret = iVec * eta + normal * (eta * cosi - sqrtf(k));
	return ret;
}


//////////////////////////////////////////////////////////////////////////
// utility

inline Vector3 splatX(Vector3Param iVec)
{
	Vector3 ret = {iVec.x, iVec.x, iVec.x};
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 splatY(Vector3Param iVec)
{
	Vector3 ret = {iVec.y, iVec.y, iVec.y};
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 splatZ(Vector3Param iVec)
{
	Vector3 ret = {iVec.z, iVec.z, iVec.z};
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 saturate(Vector3Param iVec)
{
	Vector3 ret;
	ret.x = Math::clamp(iVec.x, 0.f, 1.f);
	ret.y = Math::clamp(iVec.y, 0.f, 1.f);
	ret.z = Math::clamp(iVec.z, 0.f, 1.f);
	return ret;
}