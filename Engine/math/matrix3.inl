
//////////////////////////////////////////////////////////////////////////
// factory

inline Matrix3x3 matrix3x3(Matrix4x4Param iMat)
{
	Matrix3x3 mat;
	mat.row[0] = vector3(iMat.row[0]);
	mat.row[1] = vector3(iMat.row[1]);
	mat.row[2] = vector3(iMat.row[2]);
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix3x3 matrix3x3(const float* iData)
{
	Matrix3x3 mat;
	mat.row[0] = vector3(iData);
	mat.row[1] = vector3(iData+3);
	mat.row[2] = vector3(iData+6);
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix3x3 matrix3x3(float iM00, float iM01, float iM02,
						   float iM10, float iM11, float iM12,
						   float iM20, float iM21, float iM22,
						   float iM30, float iM31, float iM32)
{
	Matrix3x3 mat;
	mat.m00 = iM00; mat.m01 = iM01; mat.m02 = iM02;
	mat.m10 = iM10; mat.m11 = iM11; mat.m12 = iM12;
	mat.m20 = iM20; mat.m21 = iM21; mat.m22 = iM22;
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix3x3 matrix3x3(Vector3Param iRow0, Vector3Param iRow1, Vector3Param iRow2)
{
	Matrix3x3 mat;
	mat.row[0] = iRow0;
	mat.row[1] = iRow1;
	mat.row[2] = iRow2;
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix3x3 matrix3x3(const Vector3* iRows)
{
	Matrix3x3 mat;
	mat.row[0] = iRows[0];
	mat.row[1] = iRows[1];
	mat.row[2] = iRows[2];
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix3x3 identity3x3()
{
	Matrix3x3 mat;
	mat.row[0] = Vector3::kUnitX;
	mat.row[1] = Vector3::kUnitY;
	mat.row[2] = Vector3::kUnitZ;
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix3x3 scaling(Vector3Param iVec)
{
	Matrix3x3 mat;
	mat.row[0] = vector3(iVec.x, 0.0f, 0.0f);
	mat.row[1] = vector3(0.0f, iVec.y, 0.0f);
	mat.row[2] = vector3(0.0f, 0.0f, iVec.z);
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix3x3 scaling(float iX, float iY, float iZ)
{
	return scaling(vector3(iX, iY, iZ));
}

//////////////////////////////////////////////////////////////////////////

inline Matrix3x3 rotationX(float iAngle)
{
	float cosAngle = cosf(iAngle);
	float sinAngle = sinf(iAngle);

	Matrix3x3 mat;
	mat.m00 = 1.0f;
	mat.m01 = 0.0f;
	mat.m02 = 0.0f;

	mat.m10 = 0.0f;
	mat.m11 = cosAngle;
	mat.m12 = sinAngle;

	mat.m20 = 0.0f;
	mat.m21 = -sinAngle;
	mat.m22 = cosAngle;

	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix3x3 rotationY(float iAngle)
{
	float cosAngle = cosf(iAngle);
	float sinAngle = sinf(iAngle);

	Matrix3x3 mat;
	mat.m00 = cosAngle;
	mat.m01 = 0.0f;
	mat.m02 = -sinAngle;

	mat.m10 = 0.0f;
	mat.m11 = 1.0f;
	mat.m12 = 0.0f;

	mat.m20 = sinAngle;
	mat.m21 = 0.0f;
	mat.m22 = cosAngle;

	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix3x3 rotationZ(float iAngle)
{
	float cosAngle = cosf(iAngle);
	float sinAngle = sinf(iAngle);

	Matrix3x3 mat;
	mat.m00 = cosAngle;
	mat.m01 = sinAngle;
	mat.m02 = 0.0f;

	mat.m10 = -sinAngle;
	mat.m11 = cosAngle;
	mat.m12 = 0.0f;

	mat.m20 = 0.0f;
	mat.m21 = 0.0f;
	mat.m22 = 1.f;

	return mat;
}

//////////////////////////////////////////////////////////////////////////


inline Matrix3x3 rotationYawPitchRoll(Vector3Param iYawPitchRoll)
{
	return rotationYawPitchRoll(iYawPitchRoll.x, iYawPitchRoll.y, iYawPitchRoll.z);
}

inline Matrix3x3 rotationYawPitchRoll(float iYaw, float iPitch, float iRoll)
{
	// Yaw = RotateY
	// Pitch = RotateX
	// Roll = RotateZ
	// /   yaw         pitch  \       roll
	// | cy 0 -sy     1   0  0 |     cr sr 0
	// | 0  1   0  *  0  cp sp | *  -sr cr 0
	// | sy 0  cy     0 -sp cp |      0  0 1
	// \                      /
	//
	//         yaw * pitch           roll
	//      cy -sy*-sp -sy*cp       cr sr 0
	//       0    cp     sp      * -sr cr 0
	//      sy  cy*-sp  cy*cp        0  0 1
	// 
	//                   yaw * pitch * roll
	//      cy*cr-sy*-sp*-sr   cy*sr-sy*-sp*cr  -sy*cp
	//      cp*-sr             cp*cr             sp
	//      sy*cr+cy*-sp*-sr   sy*sr+cy*-sp*cr  cy*cp
	//

	float cy = cosf(iYaw);
	float sy = sinf(iYaw);
	float cp = cosf(iPitch);
	float sp = sinf(iPitch);
	float cr = cosf(iRoll);
	float sr = sinf(iRoll);

	Matrix3x3 mat;
	mat.m00 = cy*cr-sy*-sp*-sr;
	mat.m01 = cy*sr-sy*-sp*cr;
	mat.m02 = -sy*cp;

	mat.m10 = cp*-sr;
	mat.m11 = cp*cr;
	mat.m12 = sp;

	mat.m20 = sy*cr+cy*-sp*-sr;
	mat.m21 = sy*sr+cy*-sp*cr;
	mat.m22 = cy*cp;

	return mat;
}

inline Matrix3x3 rotationAxisAngle(Vector3Param iAxis, float iAngle)
{
	return rotationAxisAngle(iAxis.x, iAxis.y, iAxis.z, iAngle);
}

//////////////////////////////////////////////////////////////////////////

inline Matrix3x3 rotationAxisAngle(float iX, float iY, float iZ, float iAngle)
{

	// c * identity matrix
	// c 0 0
	// 0 c 0
	// 0 0 c
	//
	//  +
	//
	// t * symmetrical matrix
	// t*x*x t*x*y t*x*z
	// t*y*x t*y*y t*y*z
	// t*z*x t*z*y t*z*z
	//
	//  +
	//  
	// s * ~matrix (cross product in matrix form)
	// s*0   s*-rz  s*ry        wx*0  - wy*rz + wz*ry        ry * wz - wy * rz
	// s*rz  s*0    s*-rx    =  wx*rz +   0   - wz*rx   =    rz * wx - wz * rx
	// s*-ry s*rx   s*0        -wx*ry + wy*rx + wz*0         rx * wy - wx * ry

	float c = cosf(iAngle);
	float s = sinf(iAngle);
	float t = 1.f - c;

	Matrix3x3  mat;

	float x = iX;
	float y = iY;
	float z = iZ;

	mat.m00 = t*x*x + c;
	mat.m01 = t*x*y - s*z;
	mat.m02 = t*x*z + s*y;

	mat.m10 = t*y*x + s*z;
	mat.m11 = t*y*y + c;
	mat.m12 = t*y*z - s*x;

	mat.m20 = t*z*x - s*y;
	mat.m21 = t*z*y + s*x;
	mat.m22 = t*z*z + c;

	return mat;
}

//////////////////////////////////////////////////////////////////////////
// arithmetics
inline Matrix3x3 mul(Matrix3x3Param iMatA, Matrix3x3Param iMatB)
{
	Matrix3x3 result;

	float x = iMatA.m00;
	float y = iMatA.m01;
	float z = iMatA.m02;

	result.m00 = iMatB.m00 * x + iMatB.m10 * y + iMatB.m20 * z;
	result.m01 = iMatB.m01 * x + iMatB.m11 * y + iMatB.m21 * z;
	result.m02 = iMatB.m02 * x + iMatB.m12 * y + iMatB.m22 * z;

	x = iMatA.m10;
	y = iMatA.m11;
	z = iMatA.m12;

	result.m10 = iMatB.m00 * x + iMatB.m10 * y + iMatB.m20 * z;
	result.m11 = iMatB.m01 * x + iMatB.m11 * y + iMatB.m21 * z;
	result.m12 = iMatB.m02 * x + iMatB.m12 * y + iMatB.m22 * z;

	x = iMatA.m20;
	y = iMatA.m21;
	z = iMatA.m22;

	result.m20 = iMatB.m00 * x + iMatB.m10 * y + iMatB.m20 * z;
	result.m21 = iMatB.m01 * x + iMatB.m11 * y + iMatB.m21 * z;
	result.m22 = iMatB.m02 * x + iMatB.m12 * y + iMatB.m22 * z;

	return result;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix3x3 mul(Matrix3x3Param iMat, float iScalar)
{
	Matrix3x3 ret;
	for (unsigned row = 0; row < 3; ++row)
	{
		ret.row[row] = iMat.row[row] * iScalar;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix3x3& mul(Matrix3x3& iMat, float iScalar)
{
	for (unsigned row = 0; row < 3; ++row)
	{
		iMat.row[row] *= iScalar;
	}
	return iMat;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix3x3 transpose(Matrix3x3Param iMat)
{
	Matrix3x3 result;
	result.m00 = iMat.m00; result.m01 = iMat.m10; result.m02 = iMat.m20;
	result.m10 = iMat.m01; result.m11 = iMat.m11; result.m12 = iMat.m21;
	result.m20 = iMat.m02; result.m21 = iMat.m12; result.m22 = iMat.m22;
	return result;
}


inline Matrix3x3 inverse(Matrix3x3Param iMat)
{
	/*
	00 01 02
	10 11 12
	20 21 22
	*/

	float s0 = iMat.m00*iMat.m11 - iMat.m01*iMat.m10;
	float s1 = iMat.m00*iMat.m12 - iMat.m02*iMat.m10;
	float s2 = iMat.m01*iMat.m12 - iMat.m02*iMat.m11;
	float s3 = iMat.m01*iMat.m22 - iMat.m02*iMat.m21;
	float sc = iMat.m00*iMat.m22 - iMat.m02*iMat.m20;
	float c3 = iMat.m00*iMat.m21 - iMat.m01*iMat.m20;
	float c2 = iMat.m11*iMat.m22 - iMat.m12*iMat.m21;
	float c1 = iMat.m10*iMat.m22 - iMat.m12*iMat.m20;
	float c0 = iMat.m10*iMat.m21 - iMat.m11*iMat.m20;

	float invDet = 1.f / (c2*iMat.m00 - c1*iMat.m01 + c0*iMat.m02);

	Matrix3x3 adj;

	adj.m00 =  (c2) * invDet;
	adj.m10 = -(c1) * invDet;
	adj.m20 =  (c0) * invDet;

	adj.m01 = -(s3) * invDet;
	adj.m11 =  (sc) * invDet;
	adj.m21 = -(c3) * invDet;

	adj.m02 =  (s2) * invDet;
	adj.m12 = -(s1) * invDet;
	adj.m22 =  (s0) * invDet;

	return adj;
}

//////////////////////////////////////////////////////////////////////////

inline bool equal(Matrix3x3Param iMatA, Matrix3x3Param iMatB)
{
	return (equal(iMatA.row[0], iMatB.row[0]) && 
		equal(iMatA.row[1], iMatB.row[1]) && 
		equal(iMatA.row[2], iMatB.row[2]));
}

//////////////////////////////////////////////////////////////////////////

inline bool aproxEqual(Matrix3x3Param iMatA, Matrix3x3Param iMatB)
{
	return (aproxEqual(iMatA.row[0], iMatB.row[0]) && 
		aproxEqual(iMatA.row[1], iMatB.row[1]) && 
		aproxEqual(iMatA.row[2], iMatB.row[2]));
}

//////////////////////////////////////////////////////////////////////////

inline Vector3 transformVector(Matrix3x3Param iMat, Vector3Param iVec)
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

inline void transformVectorArray(Matrix3x3Param iMat, Vector3* ioArray, unsigned iCount)
{
	Matrix3x3 tMat = transpose(iMat);
	for(unsigned vecIter = 0; vecIter < iCount; ++vecIter)
	{
		float x = ioArray[vecIter].x;
		float y = ioArray[vecIter].y;
		float z = ioArray[vecIter].z;

		ioArray[vecIter].x = x*tMat.m00+y*tMat.m01+z*tMat.m02;
		ioArray[vecIter].y = x*tMat.m10+y*tMat.m11+z*tMat.m12;
		ioArray[vecIter].z = x*tMat.m20+y*tMat.m21+z*tMat.m22;
	}
}

//////////////////////////////////////////////////////////////////////////
// operators

inline bool operator==(Matrix3x3Param iMatA, Matrix3x3Param iMatB)
{
	return equal(iMatA, iMatB);
}

//////////////////////////////////////////////////////////////////////////

inline bool operator!=(Matrix3x3Param iMatA, Matrix3x3Param iMatB)
{
	return !equal(iMatA, iMatB);
}

//////////////////////////////////////////////////////////////////////////

inline Matrix3x3 operator*(Matrix3x3Param iMatA, Matrix3x3Param iMatB)
{
	return mul(iMatA, iMatB);
}

//////////////////////////////////////////////////////////////////////////

inline Matrix3x3& operator*=(Matrix3x3& iMatA, Matrix3x3Param iMatB)
{
	iMatA = mul(iMatA, iMatB);
	return iMatA;
}

//////////////////////////////////////////////////////////////////////////

inline Matrix3x3 operator*(Matrix3x3Param iMatA, float iScalar)
{
	return mul(iMatA, iScalar);
}

//////////////////////////////////////////////////////////////////////////

inline Matrix3x3& operator*=(Matrix3x3& iMatA, float iScalar)
{
	return mul(iMatA, iScalar);
}
