
//////////////////////////////////////////////////////////////////////////
// factory

inline Matrix4x4 matrix4x4(const float* iData)
{
	Matrix4x4 mat;
	mat.row[0] = vector4(iData);
	mat.row[1] = vector4(iData+4);
	mat.row[2] = vector4(iData+8);
	mat.row[3] = vector4(iData+16);
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix4x4 matrix4x4( float iM00, float iM01, float iM02, float iM03,
						   float iM10, float iM11, float iM12, float iM13,
						   float iM20, float iM21, float iM22, float iM23,
						   float iM30, float iM31, float iM32, float iM33 )
{
	Matrix4x4 mat;
	mat.m00 = iM00; mat.m01 = iM01; mat.m02 = iM02; mat.m03 = iM03;
	mat.m10 = iM10; mat.m11 = iM11; mat.m12 = iM12; mat.m13 = iM13;
	mat.m20 = iM20; mat.m21 = iM21; mat.m22 = iM22; mat.m23 = iM23;
	mat.m30 = iM30; mat.m31 = iM31; mat.m32 = iM32; mat.m33 = iM33;
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix4x4 matrix4x4(Vector4Param iRow0, Vector4Param iRow1, Vector4Param iRow2, Vector4Param iRow3)
{
	Matrix4x4 mat;
	mat.row[0] = iRow0;
	mat.row[1] = iRow1;
	mat.row[2] = iRow2;
	mat.row[3] = iRow3;
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix4x4 matrix4x4(const Vector4* iRows)
{
	Matrix4x4 mat;
	mat.row[0] = iRows[0];
	mat.row[1] = iRows[1];
	mat.row[2] = iRows[2];
	mat.row[3] = iRows[3];
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix4x4 matrix4x4(const Matrix3x3Param& iMat3)
{
	Matrix4x4 mat;
	mat.row[0] = vector4(iMat3.row[0]);
	mat.row[1] = vector4(iMat3.row[1]);
	mat.row[2] = vector4(iMat3.row[2]);
	mat.row[3] = Vector4::kUnitW;
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix4x4 identity4x4()
{
	Matrix4x4 mat;
	mat.row[0] = Vector4::kUnitX;
	mat.row[1] = Vector4::kUnitY;
	mat.row[2] = Vector4::kUnitZ;
	mat.row[3] = Vector4::kUnitW;
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix4x4 translation(Vector4Param iVec)
{
	Matrix4x4 mat;
	mat.row[0] = Vector4::kUnitX;
	mat.row[1] = Vector4::kUnitY;
	mat.row[2] = Vector4::kUnitZ;
	mat.row[3] = iVec;
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix4x4 translation(float iX, float iY, float iZ)
{
	Matrix4x4 mat;
	mat.row[0] = Vector4::kUnitX;
	mat.row[1] = Vector4::kUnitY;
	mat.row[2] = Vector4::kUnitZ;
	mat.row[3] = vector4(iX, iY, iZ, 1.f);
	return mat;
}

//////////////////////////////////////////////////////////////////////////
// arithmetics
inline Matrix4x4 mul(Matrix4x4Param iMatA, Matrix4x4Param iMatB)
{
	Matrix4x4 result;

	float x = iMatA.m00;
	float y = iMatA.m01;
	float z = iMatA.m02;
	float w = iMatA.m03;

	result.m00 = iMatB.m00 * x + iMatB.m10 * y + iMatB.m20 * z + iMatB.m30 * w;
	result.m01 = iMatB.m01 * x + iMatB.m11 * y + iMatB.m21 * z + iMatB.m31 * w;
	result.m02 = iMatB.m02 * x + iMatB.m12 * y + iMatB.m22 * z + iMatB.m32 * w;
	result.m03 = iMatB.m03 * x + iMatB.m13 * y + iMatB.m23 * z + iMatB.m33 * w;

	x = iMatA.m10;
	y = iMatA.m11;
	z = iMatA.m12;
	w = iMatA.m13;

	result.m10 = iMatB.m00 * x + iMatB.m10 * y + iMatB.m20 * z + iMatB.m30 * w;
	result.m11 = iMatB.m01 * x + iMatB.m11 * y + iMatB.m21 * z + iMatB.m31 * w;
	result.m12 = iMatB.m02 * x + iMatB.m12 * y + iMatB.m22 * z + iMatB.m32 * w;
	result.m13 = iMatB.m03 * x + iMatB.m13 * y + iMatB.m23 * z + iMatB.m33 * w;

	x = iMatA.m20;
	y = iMatA.m21;
	z = iMatA.m22;
	w = iMatA.m23;

	result.m20 = iMatB.m00 * x + iMatB.m10 * y + iMatB.m20 * z + iMatB.m30 * w;
	result.m21 = iMatB.m01 * x + iMatB.m11 * y + iMatB.m21 * z + iMatB.m31 * w;
	result.m22 = iMatB.m02 * x + iMatB.m12 * y + iMatB.m22 * z + iMatB.m32 * w;
	result.m23 = iMatB.m03 * x + iMatB.m13 * y + iMatB.m23 * z + iMatB.m33 * w;

	x = iMatA.m30;
	y = iMatA.m31;
	z = iMatA.m32;
	w = iMatA.m33;

	result.m30 = iMatB.m00 * x + iMatB.m10 * y + iMatB.m20 * z + iMatB.m30 * w;
	result.m31 = iMatB.m01 * x + iMatB.m11 * y + iMatB.m21 * z + iMatB.m31 * w;
	result.m32 = iMatB.m02 * x + iMatB.m12 * y + iMatB.m22 * z + iMatB.m32 * w;
	result.m33 = iMatB.m03 * x + iMatB.m13 * y + iMatB.m23 * z + iMatB.m33 * w;

	return result;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix4x4 mul(Matrix4x4Param iMat, float iScalar)
{
	Vector4 scalar = vector4(iScalar);
	Matrix4x4 ret = iMat;
	for (unsigned row = 0; row < 4; ++row)
	{
		ret.row[row] *= scalar;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix4x4& mul(Matrix4x4& iMat, float iScalar)
{
	Vector4 scalar = vector4(iScalar);
	for (unsigned row = 0; row < 4; ++row)
	{
		iMat.row[row] *= scalar;
	}
	return iMat;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix4x4 transpose(Matrix4x4Param iMat)
{
	Matrix4x4 result;
	result.m00 = iMat.m00; result.m01 = iMat.m10; result.m02 = iMat.m20; result.m03 = iMat.m30;
	result.m10 = iMat.m01; result.m11 = iMat.m11; result.m12 = iMat.m21; result.m13 = iMat.m31;
	result.m20 = iMat.m02; result.m21 = iMat.m12; result.m22 = iMat.m22; result.m23 = iMat.m32;
	result.m30 = iMat.m03; result.m31 = iMat.m13; result.m32 = iMat.m23; result.m33 = iMat.m33;
	return result;
}


inline Matrix4x4 inverse(Matrix4x4Param iMat)
{

	float s0 = iMat.m00*iMat.m11 - iMat.m01*iMat.m10;
	float s1 = iMat.m00*iMat.m12 - iMat.m02*iMat.m10;
	float s2 = iMat.m00*iMat.m13 - iMat.m03*iMat.m10;
	float s3 = iMat.m01*iMat.m12 - iMat.m02*iMat.m11;
	float s4 = iMat.m01*iMat.m13 - iMat.m03*iMat.m11;
	float s5 = iMat.m02*iMat.m13 - iMat.m03*iMat.m12;

	float c5 = iMat.m22*iMat.m33 - iMat.m23*iMat.m32;
	float c4 = iMat.m21*iMat.m33 - iMat.m23*iMat.m31;
	float c3 = iMat.m21*iMat.m32 - iMat.m22*iMat.m31;
	float c2 = iMat.m20*iMat.m33 - iMat.m23*iMat.m30;
	float c1 = iMat.m20*iMat.m32 - iMat.m22*iMat.m30;
	float c0 = iMat.m20*iMat.m31 - iMat.m21*iMat.m30;

	float invDet = 1.f / (s0*c5 - s1*c4 + s2*c3 + s3*c2 - s4*c1 + s5*c0);

	Matrix4x4 adj;


	adj.m00 =  (iMat.m11*c5 - iMat.m12*c4 + iMat.m13*c3) * invDet;
	adj.m10 = -(iMat.m10*c5 - iMat.m12*c2 + iMat.m13*c1) * invDet;
	adj.m20 =  (iMat.m10*c4 - iMat.m11*c2 + iMat.m13*c0) * invDet;
	adj.m30 = -(iMat.m10*c3 - iMat.m11*c1 + iMat.m12*c0) * invDet;

	adj.m01 = -(iMat.m01*c5 - iMat.m02*c4 + iMat.m03*c3) * invDet;
	adj.m11 =  (iMat.m00*c5 - iMat.m02*c2 + iMat.m03*c1) * invDet;
	adj.m21 = -(iMat.m00*c4 - iMat.m01*c2 + iMat.m03*c0) * invDet;
	adj.m31 =  (iMat.m00*c3 - iMat.m01*c1 + iMat.m02*c0) * invDet;

	adj.m02 =  (iMat.m31*s5 - iMat.m32*s4 + iMat.m33*s3) * invDet;
	adj.m12 = -(iMat.m30*s5 - iMat.m32*s2 + iMat.m33*s1) * invDet;
	adj.m22 =  (iMat.m30*s4 - iMat.m31*s2 + iMat.m33*s0) * invDet;
	adj.m32 = -(iMat.m30*s3 - iMat.m31*s1 + iMat.m32*s0) * invDet;

	adj.m03 = -(iMat.m21*s5 - iMat.m22*s4 + iMat.m23*s3) * invDet;
	adj.m13 =  (iMat.m20*s5 - iMat.m22*s2 + iMat.m23*s1) * invDet;
	adj.m23 = -(iMat.m20*s4 - iMat.m21*s2 + iMat.m23*s0) * invDet;
	adj.m33 =  (iMat.m20*s3 - iMat.m21*s1 + iMat.m22*s0) * invDet;

	return adj;

}

//////////////////////////////////////////////////////////////////////////

inline bool equal(Matrix4x4Param iMatA, Matrix4x4Param iMatB)
{
	return (equal(iMatA.row[0], iMatB.row[0]) && 
		equal(iMatA.row[1], iMatB.row[1]) && 
		equal(iMatA.row[2], iMatB.row[2]) && 
		equal(iMatA.row[3], iMatB.row[3]));
}

//////////////////////////////////////////////////////////////////////////

inline bool aproxEqual(Matrix4x4Param iMatA, Matrix4x4Param iMatB)
{
	return (aproxEqual(iMatA.row[0], iMatB.row[0]) && 
		aproxEqual(iMatA.row[1], iMatB.row[1]) && 
		aproxEqual(iMatA.row[2], iMatB.row[2]) && 
		aproxEqual(iMatA.row[3], iMatB.row[3]));
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 transform(Matrix4x4Param iMat, Vector4Param iVec)
{
	Vector4 result;

	result.x = iVec.x * iMat.row[0].x;
	result.y = iVec.x * iMat.row[0].y;
	result.z = iVec.x * iMat.row[0].z;
	result.w = iVec.x * iMat.row[0].w;

	result.x = iVec.y * iMat.row[1].x + result.x;
	result.y = iVec.y * iMat.row[1].y + result.y;
	result.z = iVec.y * iMat.row[1].z + result.z;
	result.w = iVec.y * iMat.row[1].w + result.w;

	result.x = iVec.z * iMat.row[2].x + result.x;
	result.y = iVec.z * iMat.row[2].y + result.y;
	result.z = iVec.z * iMat.row[2].z + result.z;
	result.w = iVec.z * iMat.row[2].w + result.w;

	result.x = iVec.w * iMat.row[3].x + result.x;
	result.y = iVec.w * iMat.row[3].y + result.y;
	result.z = iVec.w * iMat.row[3].z + result.z;
	result.w = iVec.w * iMat.row[3].w + result.w;

	return result;
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 transformCoord(Matrix4x4Param iMat, Vector4Param iVec)
{
	Vector4 result;

	result.x = iVec.x * iMat.row[0].x;
	result.y = iVec.x * iMat.row[0].y;
	result.z = iVec.x * iMat.row[0].z;
	result.w = iVec.x * iMat.row[0].w;

	result.x = iVec.y * iMat.row[1].x + result.x;
	result.y = iVec.y * iMat.row[1].y + result.y;
	result.z = iVec.y * iMat.row[1].z + result.z;
	result.w = iVec.y * iMat.row[1].w + result.w;

	result.x = iVec.z * iMat.row[2].x + result.x;
	result.y = iVec.z * iMat.row[2].y + result.y;
	result.z = iVec.z * iMat.row[2].z + result.z;
	result.w = iVec.z * iMat.row[2].w + result.w;

	result.x = iMat.row[3].x + result.x;
	result.y = iMat.row[3].y + result.y;
	result.z = iMat.row[3].z + result.z;
	result.w = iMat.row[3].w + result.w;

	float recipW = 1.f / result.w;

	result.x = result.x * recipW;
	result.y = result.y * recipW;
	result.z = result.z * recipW;
	result.w = result.w * recipW;

	return result;
}

//////////////////////////////////////////////////////////////////////////

inline Vector4 transformVector(Matrix4x4Param iMat, Vector4Param iVec)
{
	Vector4 result;

	result.x = iVec.x * iMat.row[0].x;
	result.y = iVec.x * iMat.row[0].y;
	result.z = iVec.x * iMat.row[0].z;
	result.w = 0.f;

	result.x = iVec.y * iMat.row[1].x + result.x;
	result.y = iVec.y * iMat.row[1].y + result.y;
	result.z = iVec.y * iMat.row[1].z + result.z;

	result.x = iVec.z * iMat.row[2].x + result.x;
	result.y = iVec.z * iMat.row[2].y + result.y;
	result.z = iVec.z * iMat.row[2].z + result.z;

	return result;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 transformCoord(Matrix4x4Param iMat, Vector3Param iVec)
{
	Vector3 result;

	result.x = iVec.x * iMat.row[0].x;
	result.y = iVec.x * iMat.row[0].y;
	result.z = iVec.x * iMat.row[0].z;
	float w  = iVec.x * iMat.row[0].w;

	result.x = iVec.y * iMat.row[1].x + result.x;
	result.y = iVec.y * iMat.row[1].y + result.y;
	result.z = iVec.y * iMat.row[1].z + result.z;
		   w = iVec.y * iMat.row[1].w + w;

	result.x = iVec.z * iMat.row[2].x + result.x;
	result.y = iVec.z * iMat.row[2].y + result.y;
	result.z = iVec.z * iMat.row[2].z + result.z;
		   w = iVec.z * iMat.row[2].w + w;

	result.x = iMat.row[3].x + result.x;
	result.y = iMat.row[3].y + result.y;
	result.z = iMat.row[3].z + result.z;
		   w = iMat.row[3].w + w;

	float recipW = 1.f / w;

	result.x = result.x * recipW;
	result.y = result.y * recipW;
	result.z = result.z * recipW;

	return result;
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 transformVector(Matrix4x4Param iMat, Vector3Param iVec)
{
	Vector3 result;

	result.x = iVec.x * iMat.row[0].x;
	result.y = iVec.x * iMat.row[0].y;
	result.z = iVec.x * iMat.row[0].z;

	result.x = iVec.y * iMat.row[1].x + result.x;
	result.y = iVec.y * iMat.row[1].y + result.y;
	result.z = iVec.y * iMat.row[1].z + result.z;

	result.x = iVec.z * iMat.row[2].x + result.x;
	result.y = iVec.z * iMat.row[2].y + result.y;
	result.z = iVec.z * iMat.row[2].z + result.z;

	return result;
}

//////////////////////////////////////////////////////////////////////////

inline void transformArray(Matrix4x4Param iMat, Vector4* ioArray, unsigned iCount)
{
	Matrix4x4 tMat = transpose(iMat);
	for(unsigned vecIter = 0; vecIter < iCount; ++vecIter)
	{
		float x = ioArray[vecIter].x;
		float y = ioArray[vecIter].y;
		float z = ioArray[vecIter].z;
		float w = ioArray[vecIter].w;

		ioArray[vecIter].x = x*tMat.m00+y*tMat.m01+z*tMat.m02+w*tMat.m03;
		ioArray[vecIter].y = x*tMat.m10+y*tMat.m11+z*tMat.m12+w*tMat.m13;
		ioArray[vecIter].z = x*tMat.m20+y*tMat.m21+z*tMat.m22+w*tMat.m23;
		ioArray[vecIter].w = x*tMat.m30+y*tMat.m31+z*tMat.m32+w*tMat.m33;
	}
}

//////////////////////////////////////////////////////////////////////////

inline void transformCoordArray(Matrix4x4Param iMat, Vector4* ioArray, unsigned iCount)
{
	Matrix4x4 tMat = transpose(iMat);
	for(unsigned vecIter = 0; vecIter < iCount; ++vecIter)
	{
		float x = ioArray[vecIter].x;
		float y = ioArray[vecIter].y;
		float z = ioArray[vecIter].z;

		ioArray[vecIter].x = x*tMat.m00+y*tMat.m01+z*tMat.m02+tMat.m03;
		ioArray[vecIter].y = x*tMat.m10+y*tMat.m11+z*tMat.m12+tMat.m13;
		ioArray[vecIter].z = x*tMat.m20+y*tMat.m21+z*tMat.m22+tMat.m23;
		ioArray[vecIter].w = x*tMat.m30+y*tMat.m31+z*tMat.m32+tMat.m33;

		float recipW = 1.f / ioArray[vecIter].w;
		ioArray[vecIter] *= recipW;
	}
}

//////////////////////////////////////////////////////////////////////////

inline void transformVectorArray(Matrix4x4Param iMat, Vector4* ioArray, unsigned iCount)
{
	Matrix4x4 tMat = transpose(iMat);
	for(unsigned vecIter = 0; vecIter < iCount; ++vecIter)
	{
		float x = ioArray[vecIter].x;
		float y = ioArray[vecIter].y;
		float z = ioArray[vecIter].z;

		ioArray[vecIter].x = x*tMat.m00+y*tMat.m01+z*tMat.m02;
		ioArray[vecIter].y = x*tMat.m10+y*tMat.m11+z*tMat.m12;
		ioArray[vecIter].z = x*tMat.m20+y*tMat.m21+z*tMat.m22;
		ioArray[vecIter].w = 0.f;
	}
}

//////////////////////////////////////////////////////////////////////////

//inline Matrix4x4 lookAtLH(Vector4Param iEyePoint, Vector4Param iFocusPoint, Vector4Param iUpVec)
//{
//
//}
//
////////////////////////////////////////////////////////////////////////////
//
//inline Matrix4x4 lookAtRH(Vector4Param iEyePoint, Vector4Param iFocusPoint, Vector4Param iUpVec)
//{
//
//}

inline Matrix4x4 lookAtRH(Vector3Param iEyePoint, Vector3Param iFocusPoint, Vector3Param iUpVec)
{
	Vector3 zaxis = normalize(iEyePoint - iFocusPoint);
	Vector3 xaxis = cross(iUpVec, zaxis);
	Vector3 yaxis = cross(zaxis, xaxis);

	Matrix4x4 ret =
	{
		xaxis.x, xaxis.y, xaxis.z, 0,
		yaxis.x, yaxis.y, yaxis.z, 0,
		zaxis.x, zaxis.y, zaxis.z, 0,
		-dot(xaxis, iEyePoint), -dot(yaxis, iEyePoint), -dot(zaxis, iEyePoint), 1
	};
	return ret;
}


//////////////////////////////////////////////////////////////////////////

inline Matrix4x4 perspectiveFovLH(float iFovDeg, float iAspect, float iNear, float iFar)
{
	float invHalfTan = 1.f / tanf( Math::radians(iFovDeg) * 0.5f );

	Matrix4x4 persp = identity4x4();
	persp.m00 = invHalfTan / iAspect;
	persp.m11 = invHalfTan;
	persp.m22 = iFar / (iFar - iNear);
	persp.m32 = -iFar * iNear / (iFar - iNear);
	persp.m23 = 1;
	persp.m33 = 0;

	return persp;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix4x4 perspectiveFovRH(float iFovDeg, float iAspect, float iNear, float iFar)
{
	float invHalfTan = 1.f / tanf(Math::radians(iFovDeg) * 0.5f);

	Matrix4x4 persp = identity4x4();
	persp.m00 = invHalfTan / iAspect;
	persp.m11 = invHalfTan;
	persp.m22 = iFar / (iNear - iFar);
	persp.m32 = iNear * iFar / (iNear - iFar);
	persp.m23 = -1;
	persp.m33 = 0;

	return persp;
}

//////////////////////////////////////////////////////////////////////////
// operators

inline bool operator==(Matrix4x4Param iMatA, Matrix4x4Param iMatB)
{
	return equal(iMatA, iMatB);
}

//////////////////////////////////////////////////////////////////////////

inline bool operator!=(Matrix4x4Param iMatA, Matrix4x4Param iMatB)
{
	return !equal(iMatA, iMatB);
}

//////////////////////////////////////////////////////////////////////////

inline Matrix4x4 operator*(Matrix4x4Param iMatA, Matrix4x4Param iMatB)
{
	return mul(iMatA, iMatB);
}

//////////////////////////////////////////////////////////////////////////

inline Matrix4x4& operator*=(Matrix4x4& iMatA, Matrix4x4Param iMatB)
{
	iMatA = mul(iMatA, iMatB);
	return iMatA;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix4x4 operator*(Matrix4x4Param iMatA, float iScalar)
{
	return mul(iMatA, iScalar);
}

//////////////////////////////////////////////////////////////////////////

inline Matrix4x4& operator*=(Matrix4x4& iMatA, float iScalar)
{
	return mul(iMatA, iScalar);
}
