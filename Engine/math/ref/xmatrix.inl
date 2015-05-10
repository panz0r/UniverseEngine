
//////////////////////////////////////////////////////////////////////////
// factory

inline XMatrix xmatrix(const float* iData)
{
	XMatrix mat;
	mat.row[0] = xvector(iData);
	mat.row[1] = xvector(iData+4);
	mat.row[2] = xvector(iData+8);
	mat.row[3] = xvector(iData+16);
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix xmatrix( float iM00, float iM01, float iM02, float iM03,
					   float iM10, float iM11, float iM12, float iM13,
					   float iM20, float iM21, float iM22, float iM23,
					   float iM30, float iM31, float iM32, float iM33 )
{
	XMatrix mat;
	mat.row[0] = xvector(iM00, iM01, iM02, iM03);
	mat.row[1] = xvector(iM10, iM11, iM12, iM13);
	mat.row[2] = xvector(iM20, iM21, iM22, iM23);
	mat.row[3] = xvector(iM30, iM31, iM32, iM33);
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix xmatrix(XVectorParam iRow0, XVectorParam iRow1, XVectorParam iRow2, XVectorParam iRow3)
{
	XMatrix mat;
	mat.row[0] = iRow0;
	mat.row[1] = iRow1;
	mat.row[2] = iRow2;
	mat.row[3] = iRow3;
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix xmatrix(const XVector* iRows)
{
	XMatrix mat;
	mat.row[0] = iRows[0];
	mat.row[1] = iRows[1];
	mat.row[2] = iRows[2];
	mat.row[3] = iRows[3];
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix xidentity()
{
	XMatrix mat;
	mat.row[0] = xvector(Vector4::kUnitX);
	mat.row[1] = xvector(Vector4::kUnitY);
	mat.row[2] = xvector(Vector4::kUnitZ);
	mat.row[3] = xvector(Vector4::kUnitW);
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix xtranslation(XVectorParam iVec)
{
	XMatrix mat;
	mat.row[0] = xvector(Vector4::kUnitX);
	mat.row[1] = xvector(Vector4::kUnitY);
	mat.row[2] = xvector(Vector4::kUnitZ);
	mat.row[3] = iVec;
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix xtranslation(float iX, float iY, float iZ)
{
	XMatrix mat = xtranslation(xvector(iX, iY, iZ, 1.f));
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix xscaling(XVectorParam iVec)
{
	XMatrix mat;
	mat.row[0] = xvector(iVec.x, 0.0f, 0.0f, 0.0f);
	mat.row[1] = xvector(0.0f, iVec.y, 0.0f, 0.0f);
	mat.row[2] = xvector(0.0f, 0.0f, iVec.z, 0.0f);
	mat.row[3] = xvector(Vector4::kUnitW);
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix xscaling(float iX, float iY, float iZ)
{
	XMatrix mat;
	mat.row[0] = xvector(iX, 0.0f, 0.0f, 0.0f);
	mat.row[1] = xvector(0.0f, iY, 0.0f, 0.0f);
	mat.row[2] = xvector(0.0f, 0.0f, iZ, 0.0f);
	mat.row[3] = xvector(Vector4::kUnitW);
	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix xrotationX(float iAngle)
{
	float cosAngle = cosf(iAngle);
	float sinAngle = sinf(iAngle);

	XMatrix mat;

	mat.m00 = 1.0f;
	mat.m01 = 0.0f;
	mat.m02 = 0.0f;
	mat.m03 = 0.0f;

	mat.m10 = 0.0f;
	mat.m11 = cosAngle;
	mat.m12 = sinAngle;
	mat.m13 = 0.0f;

	mat.m20 = 0.0f;
	mat.m21 = -sinAngle;
	mat.m22 = cosAngle;
	mat.m23 = 0.0f;

	mat.m30 = 0.0f;
	mat.m31 = 0.0f;
	mat.m32 = 0.0f;
	mat.m33 = 1.0f;

	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix xrotationY(float iAngle)
{
	float cosAngle = cosf(iAngle);
	float sinAngle = sinf(iAngle);

	XMatrix mat;

	mat.m00 = cosAngle;
	mat.m01 = 0.0f;
	mat.m02 = -sinAngle;
	mat.m03 = 0.0f;

	mat.m10 = 0.0f;
	mat.m11 = 1.0f;
	mat.m12 = 0.0f;
	mat.m13 = 0.0f;

	mat.m20 = sinAngle;
	mat.m21 = 0.0f;
	mat.m22 = cosAngle;
	mat.m23 = 0.0f;

	mat.m30 = 0.0f;
	mat.m31 = 0.0f;
	mat.m32 = 0.0f;
	mat.m33 = 1.0f;

	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix xrotationZ(float iAngle)
{
	float cosAngle = cosf(iAngle);
	float sinAngle = sinf(iAngle);

	XMatrix mat;

	mat.m00 = cosAngle;
	mat.m01 = sinAngle;
	mat.m02 = 0.0f;
	mat.m03 = 0.0f;

	mat.m10 = -sinAngle;
	mat.m11 = cosAngle;
	mat.m12 = 0.0f;
	mat.m13 = 0.0f;

	mat.m20 = 0.0f;
	mat.m21 = 0.0f;
	mat.m22 = 1.f;
	mat.m23 = 0.0f;

	mat.m30 = 0.0f;
	mat.m31 = 0.0f;
	mat.m32 = 0.0f;
	mat.m33 = 1.0f;

	return mat;
}

//////////////////////////////////////////////////////////////////////////


inline XMatrix xrotationYawPitchRoll(XVectorParam iYawPitchRoll)
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

	float yaw = iYawPitchRoll.x;
	float pitch = iYawPitchRoll.y;
	float roll = iYawPitchRoll.z;

	float cy = cosf(yaw);
	float sy = sinf(yaw);
	float cp = cosf(pitch);
	float sp = sinf(pitch);
	float cr = cosf(roll);
	float sr = sinf(roll);

	XMatrix mat;
	mat.m00 = cy*cr-sy*-sp*-sr;
	mat.m01 = cy*sr-sy*-sp*cr;
	mat.m02 = -sy*cp;
	mat.m03 = 0;

	mat.m10 = cp*-sr;
	mat.m11 = cp*cr;
	mat.m12 = sp;
	mat.m13 = 0;

	mat.m20 = sy*cr+cy*-sp*-sr;
	mat.m21 = sy*sr+cy*-sp*cr;
	mat.m22 = cy*cp;
	mat.m23 = 0;

	mat.m30 = 0;
	mat.m31 = 0;
	mat.m32 = 0;
	mat.m33 = 1.f;

	return mat;
}

inline XMatrix xrotationYawPitchRoll(float iYaw, float iPitch, float iRoll)
{
	return rotationYawPitchRoll(xvector(iYaw, iPitch, iRoll, 0.f));
}

inline XMatrix xrotationAxisAngle(XVectorParam iAxis, float iAngle)
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

	XMatrix  mat;

	float x = iAxis.x;
	float y = iAxis.y;
	float z = iAxis.z;

	mat.m00 = t*x*x + c;
	mat.m01 = t*x*y - s*z;
	mat.m02 = t*x*z + s*y;
	mat.m03 = 0.f;

	mat.m10 = t*y*x + s*z;
	mat.m11 = t*y*y + c;
	mat.m12 = t*y*z - s*x;
	mat.m13 = 0.f;

	mat.m20 = t*z*x - s*y;
	mat.m21 = t*z*y + s*x;
	mat.m22 = t*z*z + c;
	mat.m23 = 0.f;

	mat.m30 = 0.f;
	mat.m31 = 0.f;
	mat.m32 = 0.f;
	mat.m33 = 1.f;

	return mat;
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix xrotationAxisAngle(float iX, float iY, float iZ, float iAngle)
{
	return xrotationAxisAngle(xvector(iX, iY, iZ, 0.f), iAngle);
}

//////////////////////////////////////////////////////////////////////////
// arithmetics
inline XMatrix mul(XMatrixParam iMatA, XMatrixParam iMatB)
{
	XMatrix result;

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

inline XMatrix mul(XMatrixParam iMat, float iScalar)
{
	XVector scalar = xvector(iScalar);
	XMatrix ret = iMat;
	for (uint row = 0; row < 4; ++row)
	{
		ret.row[row] *= scalar;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix& mul(XMatrix& iMat, float iScalar)
{
	XVector scalar = xvector(iScalar);
	for (uint row = 0; row < 4; ++row)
	{
		iMat.row[row] *= scalar;
	}
	return iMat;
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix transpose(XMatrixParam iMat)
{
	XMatrix result;
	result.m00 = iMat.m00; result.m01 = iMat.m10; result.m02 = iMat.m20; result.m03 = iMat.m30;
	result.m10 = iMat.m01; result.m11 = iMat.m11; result.m12 = iMat.m21; result.m13 = iMat.m31;
	result.m20 = iMat.m02; result.m21 = iMat.m12; result.m22 = iMat.m22; result.m23 = iMat.m32;
	result.m30 = iMat.m03; result.m31 = iMat.m13; result.m32 = iMat.m23; result.m33 = iMat.m33;
	return result;
}


inline XMatrix inverse(XMatrixParam iMat)
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

	XMatrix adj;


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

inline bool equal(XMatrixParam iMatA, XMatrixParam iMatB)
{
	return (equal(iMatA.row[0], iMatB.row[0]) && 
		equal(iMatA.row[1], iMatB.row[1]) && 
		equal(iMatA.row[2], iMatB.row[2]) && 
		equal(iMatA.row[3], iMatB.row[3]));
}

//////////////////////////////////////////////////////////////////////////

inline bool aproxEqual(XMatrixParam iMatA, XMatrixParam iMatB)
{
	return (aproxEqual(iMatA.row[0], iMatB.row[0]) && 
		aproxEqual(iMatA.row[1], iMatB.row[1]) && 
		aproxEqual(iMatA.row[2], iMatB.row[2]) && 
		aproxEqual(iMatA.row[3], iMatB.row[3]));
}

//////////////////////////////////////////////////////////////////////////

inline XVector transform(XMatrixParam iMat, XVectorParam iVec)
{
	XVector result;

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

inline XVector transformCoord(XMatrixParam iMat, XVectorParam iVec)
{
	XVector result;

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

inline XVector transformVector(XMatrixParam iMat, XVectorParam iVec)
{
	XVector result;

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

	return result;
}

//////////////////////////////////////////////////////////////////////////

inline void transformArray(XMatrixParam iMat, XVector* ioArray, uint iCount)
{
	XMatrix tMat = transpose(iMat);
	for(uint vecIter = 0; vecIter < iCount; ++vecIter)
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

inline void transformCoordArray(XMatrixParam iMat, XVector* ioArray, uint iCount)
{
	XMatrix tMat = transpose(iMat);
	for(uint vecIter = 0; vecIter < iCount; ++vecIter)
	{
		float x = ioArray[vecIter].x;
		float y = ioArray[vecIter].y;
		float z = ioArray[vecIter].z;
		float w = ioArray[vecIter].w;

		ioArray[vecIter].x = x*tMat.m00+y*tMat.m01+z*tMat.m02+tMat.m03;
		ioArray[vecIter].y = x*tMat.m10+y*tMat.m11+z*tMat.m12+tMat.m13;
		ioArray[vecIter].z = x*tMat.m20+y*tMat.m21+z*tMat.m22+tMat.m23;
		ioArray[vecIter].w = x*tMat.m30+y*tMat.m31+z*tMat.m32+tMat.m33;

		float recipW = 1.f / ioArray[vecIter].w;
		ioArray[vecIter] *= recipW;
	}
}

//////////////////////////////////////////////////////////////////////////

inline void transformVectorArray(XMatrixParam iMat, XVector* ioArray, uint iCount)
{
	XMatrix tMat = transpose(iMat);
	for(uint vecIter = 0; vecIter < iCount; ++vecIter)
	{
		float x = ioArray[vecIter].x;
		float y = ioArray[vecIter].y;
		float z = ioArray[vecIter].z;
		float w = ioArray[vecIter].w;

		ioArray[vecIter].x = x*tMat.m00+y*tMat.m01+z*tMat.m02;
		ioArray[vecIter].y = x*tMat.m10+y*tMat.m11+z*tMat.m12;
		ioArray[vecIter].z = x*tMat.m20+y*tMat.m21+z*tMat.m22;
		ioArray[vecIter].w = x*tMat.m30+y*tMat.m31+z*tMat.m32;
	}
}

//////////////////////////////////////////////////////////////////////////
// operators

inline bool operator==(XMatrixParam iMatA, XMatrixParam iMatB)
{
	return equal(iMatA, iMatB);
}

//////////////////////////////////////////////////////////////////////////

inline bool operator!=(XMatrixParam iMatA, XMatrixParam iMatB)
{
	return !equal(iMatA, iMatB);
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix operator*(XMatrixParam iMatA, XMatrixParam iMatB)
{
	return mul(iMatA, iMatB);
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix& operator*=(XMatrix& iMatA, XMatrixParam iMatB)
{
	iMatA = mul(iMatA, iMatB);
	return iMatA;
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix operator*(XMatrixParam iMatA, float iScalar)
{
	return mul(iMatA, iScalar);
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix& operator*=(XMatrix& iMatA, float iScalar)
{
	return mul(iMatA, iScalar);
}
