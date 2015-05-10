
//////////////////////////////////////////////////////////////////////////
// factory
inline XMatrix xmatrix(Matrix4x4Param iMat)
{
	XMatrix mat = xmatrix(iMat.v);
}

//////////////////////////////////////////////////////////////////////////

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

inline XMatrix xmatrix(XVectorParam iRow0, XVectorParam iRow1, XVectorParam iRow2, const XVector& iRow3)
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
	mat.row[0] = _mm_mul_ps(iVec, xvector(Vector4::kUnitX));
	mat.row[1] = _mm_mul_ps(iVec, xvector(Vector4::kUnitY));
	mat.row[2] = _mm_mul_ps(iVec, xvector(Vector4::kUnitZ));
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
	XVector tmp = xvector(sinAngle, cosAngle, -sinAngle, 0);

	XMatrix mat;
	mat.row[0] = xvector(Vector4::kUnitX);
	mat.row[1] = _mm_shuffle_ps(tmp, tmp, EM_SHUFFLE(3,1,0,3));
	mat.row[2] = _mm_shuffle_ps(tmp, tmp, EM_SHUFFLE(3,2,1,3));
	mat.row[3] = xvector(Vector4::kUnitW);

	//mat.m00 = 1.0f;
	//mat.m01 = 0.0f;
	//mat.m02 = 0.0f;
	//mat.m03 = 0.0f;

	//mat.m10 = 0.0f;
	//mat.m11 = cosAngle;
	//mat.m12 = sinAngle;
	//mat.m13 = 0.0f;

	//mat.m20 = 0.0f;
	//mat.m21 = -sinAngle;
	//mat.m22 = cosAngle;
	//mat.m23 = 0.0f;

	//mat.m30 = 0.0f;
	//mat.m31 = 0.0f;
	//mat.m32 = 0.0f;
	//mat.m33 = 1.0f;

	return mat;
}


//////////////////////////////////////////////////////////////////////////

inline XMatrix xrotationY(float iAngle)
{
	float cosAngle = cosf(iAngle);
	float sinAngle = sinf(iAngle);
	XVector tmp = xvector(sinAngle, cosAngle, -sinAngle, 0);

	XMatrix mat;
	mat.row[0] = _mm_shuffle_ps(tmp, tmp, EM_SHUFFLE(1,3,2,3));
	mat.row[1] = xvector(Vector4::kUnitY);
	mat.row[2] = _mm_shuffle_ps(tmp, tmp, EM_SHUFFLE(0,3,1,3));
	mat.row[3] = xvector(Vector4::kUnitW);

	//mat.m00 = cosAngle;
	//mat.m01 = 0.0f;
	//mat.m02 = -sinAngle;
	//mat.m03 = 0.0f;

	//mat.m10 = 0.0f;
	//mat.m11 = 1.0f;
	//mat.m12 = 0.0f;
	//mat.m13 = 0.0f;

	//mat.m20 = sinAngle;
	//mat.m21 = 0.0f;
	//mat.m22 = cosAngle;
	//mat.m23 = 0.0f;

	//mat.m30 = 0.0f;
	//mat.m31 = 0.0f;
	//mat.m32 = 0.0f;
	//mat.m33 = 1.0f;

	return mat;
}


//////////////////////////////////////////////////////////////////////////

inline XMatrix xrotationZ(float iAngle)
{
	float cosAngle = cosf(iAngle);
	float sinAngle = sinf(iAngle);
	XVector tmp = xvector(sinAngle, cosAngle, -sinAngle, 0);

	XMatrix mat;
	mat.row[0] = _mm_shuffle_ps(tmp, tmp, EM_SHUFFLE(1,0,3,3));
	mat.row[1] = _mm_shuffle_ps(tmp, tmp, EM_SHUFFLE(2,1,3,3));
	mat.row[2] = xvector(Vector4::kUnitZ);
	mat.row[3] = xvector(Vector4::kUnitW);

	//mat.m00 = cosAngle;
	//mat.m01 = sinAngle;
	//mat.m02 = 0.0f;
	//mat.m03 = 0.0f;

	//mat.m10 = -sinAngle;
	//mat.m11 = cosAngle;
	//mat.m12 = 0.0f;
	//mat.m13 = 0.0f;

	//mat.m20 = 0.0f;
	//mat.m21 = 0.0f;
	//mat.m22 = 1.f;
	//mat.m23 = 0.0f;

	//mat.m30 = 0.0f;
	//mat.m31 = 0.0f;
	//mat.m32 = 0.0f;
	//mat.m33 = 1.0f;

	return mat;
}

//////////////////////////////////////////////////////////////////////////

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

	//float x = iAxis.x;
	//float y = iAxis.y;
	//float z = iAxis.z;

	__m128 _tmp = _mm_shuffle_ps(iAxis, xvector(Vector4::kOne), EM_SHUFFLE(2,2,0,0));
	__m128 _xyz1 = _mm_shuffle_ps(iAxis, _tmp, EM_SHUFFLE(0,1,0,3)); //_mm_set_ps(1.f, z, y, x);
	__m128 _ttt0 = _mm_set_ps(0.f, t, t, t);
	__m128 _sssc = _mm_set_ps(c, s, s, s);
	//__m128 _cccc = _mm_shuffle_ps(_sssc,_sssc, EM_SHUFFLE(3,3,3,3));

	__m128 sxsyszc1 = _mm_mul_ps(_sssc, _xyz1);
	__m128 txtytz0 = _mm_mul_ps(_ttt0, _xyz1);

	{ 
		__m128 _0_sy_nsz_c = _mm_mul_ps(sxsyszc1, _mm_set_ps(1.f, -1.f, 1.f, 0.f) );
		__m128 c_nsz_sy_0 =  _mm_shuffle_ps(_0_sy_nsz_c, _0_sy_nsz_c, EM_SHUFFLE(3,2,1,0));
		__m128 _xxx1 = _mm_shuffle_ps(_xyz1, _xyz1, EM_SHUFFLE(0,0,0,3));
		__m128 txxtxytxz0 = _mm_mul_ps(txtytz0, _xxx1);
		mat.row[0] = _mm_add_ps(txxtxytxz0, c_nsz_sy_0);
		//mat.m00 = t*x*x + c;
		//mat.m01 = t*x*y - s*z;
		//mat.m02 = t*x*z + s*y;
		//mat.m03 = 0.f;
	}
	{
		__m128 _nsx_0_sz_c = _mm_mul_ps(sxsyszc1, _mm_set_ps(-1.f, 0.f, 1.f, 1.f) );
		__m128 sz_c_nsx_0 =  _mm_shuffle_ps(_nsx_0_sz_c, _nsx_0_sz_c, EM_SHUFFLE(2,3,0,1));
		__m128 yyy1 = _mm_shuffle_ps(_xyz1, _xyz1, EM_SHUFFLE(1,1,1,3));
		__m128 tyxtyytyz0 = _mm_mul_ps(txtytz0, yyy1);
		mat.row[1] = _mm_add_ps(tyxtyytyz0, sz_c_nsx_0);
		//mat.m10 = t*y*x + s*z;
		//mat.m11 = t*y*y + c;
		//mat.m12 = t*y*z - s*x;
		//mat.m13 = 0.f;
	}
	{
		__m128 _sx_nsy_0_c = _mm_mul_ps(sxsyszc1, _mm_set_ps(1.f, -1.f, 0.f, 1.f) );
		__m128 nsy_sx_c_0 =  _mm_shuffle_ps(_sx_nsy_0_c, _sx_nsy_0_c, EM_SHUFFLE(1,0,3,2));
		__m128 zzz1 = _mm_shuffle_ps(_xyz1, _xyz1, EM_SHUFFLE(2,2,2,3));
		__m128 tzxtzytzz0 = _mm_mul_ps(txtytz0, zzz1);
		mat.row[2] = _mm_add_ps(tzxtzytzz0, nsy_sx_c_0);
		//mat.m20 = t*z*x - s*y;
		//mat.m21 = t*z*y + s*x;
		//mat.m22 = t*z*z + c;
		//mat.m23 = 0.f;
	}

	mat.row[3] = xvector(Vector4::kUnitW);
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

	XVector x = splatX(iMatA.row[0]);
	XVector y = splatY(iMatA.row[0]);
	XVector z = splatZ(iMatA.row[0]);
	XVector w = splatW(iMatA.row[0]);

	XVector prod1 = _mm_mul_ps(iMatB.row[0], x);
	XVector prod2 = _mm_mul_ps(iMatB.row[1], y);
	XVector prod3 = _mm_mul_ps(iMatB.row[2], z);
	XVector prod4 = _mm_mul_ps(iMatB.row[3], w);
	result.row[0] = _mm_add_ps( _mm_add_ps(prod1, prod2), _mm_add_ps(prod3, prod4) );

	x = splatX(iMatA.row[1]);
	y = splatY(iMatA.row[1]);
	z = splatZ(iMatA.row[1]);
	w = splatW(iMatA.row[1]);

	prod1 = _mm_mul_ps(iMatB.row[0], x);
	prod2 = _mm_mul_ps(iMatB.row[1], y);
	prod3 = _mm_mul_ps(iMatB.row[2], z);
	prod4 = _mm_mul_ps(iMatB.row[3], w);
	result.row[1] = _mm_add_ps( _mm_add_ps(prod1, prod2), _mm_add_ps(prod3, prod4) );


	x = splatX(iMatA.row[2]);
	y = splatY(iMatA.row[2]);
	z = splatZ(iMatA.row[2]);
	w = splatW(iMatA.row[2]);

	prod1 = _mm_mul_ps(iMatB.row[0], x);
	prod2 = _mm_mul_ps(iMatB.row[1], y);
	prod3 = _mm_mul_ps(iMatB.row[2], z);
	prod4 = _mm_mul_ps(iMatB.row[3], w);
	result.row[2] = _mm_add_ps( _mm_add_ps(prod1, prod2), _mm_add_ps(prod3, prod4) );

	x = splatX(iMatA.row[3]);
	y = splatY(iMatA.row[3]);
	z = splatZ(iMatA.row[3]);
	w = splatW(iMatA.row[3]);

	prod1 = _mm_mul_ps(iMatB.row[0], x);
	prod2 = _mm_mul_ps(iMatB.row[1], y);
	prod3 = _mm_mul_ps(iMatB.row[2], z);
	prod4 = _mm_mul_ps(iMatB.row[3], w);
	result.row[3] = _mm_add_ps( _mm_add_ps(prod1, prod2), _mm_add_ps(prod3, prod4) );

	return result;
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix mul(XMatrixParam iMat, float iScalar)
{
	XVector scalar = xvector(iScalar);
	XMatrix ret;
	ret.row[0] = _mm_mul_ps(iMat.row[0], scalar);
	ret.row[1] = _mm_mul_ps(iMat.row[1], scalar);
	ret.row[2] = _mm_mul_ps(iMat.row[2], scalar);
	ret.row[3] = _mm_mul_ps(iMat.row[3], scalar);
	return ret;
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix& mul(XMatrix& iMat, float iScalar)
{
	XVector scalar = xvector(iScalar);
	iMat.row[0] = _mm_mul_ps(iMat.row[0], scalar);
	iMat.row[1] = _mm_mul_ps(iMat.row[1], scalar);
	iMat.row[2] = _mm_mul_ps(iMat.row[2], scalar);
	iMat.row[3] = _mm_mul_ps(iMat.row[3], scalar);
	return iMat;
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix transpose(XMatrixParam iMat)
{
	XVector tmp0 = _mm_shuffle_ps(iMat.row[0], iMat.row[1], EM_SHUFFLE(0,1,0,1));
	XVector tmp2 = _mm_shuffle_ps(iMat.row[0], iMat.row[1], EM_SHUFFLE(2,3,2,3));
	XVector tmp1 = _mm_shuffle_ps(iMat.row[2], iMat.row[3], EM_SHUFFLE(0,1,0,1));
	XVector tmp3 = _mm_shuffle_ps(iMat.row[2], iMat.row[3], EM_SHUFFLE(2,3,2,3));
	XMatrix result;
	result.row[0] = _mm_shuffle_ps(tmp0, tmp1, EM_SHUFFLE(0,2,0,2));
	result.row[1] = _mm_shuffle_ps(tmp0, tmp1, EM_SHUFFLE(1,3,1,3));
	result.row[2] = _mm_shuffle_ps(tmp2, tmp3, EM_SHUFFLE(0,2,0,2));
	result.row[3] = _mm_shuffle_ps(tmp2, tmp3, EM_SHUFFLE(1,3,1,3));
	return result;
}

//////////////////////////////////////////////////////////////////////////

inline XMatrix inverse(XMatrixParam iMat)
{

	//           2x2 determinants 
	/////////////////////////////////////////
	//00 01      //00    02   //00       03//
	//10 11      //10    12   //10       13//
	//      22 23//   21    23//   21 22   //
	//      32 33//   31 	33//   31 32   //
	/////////////////////////////////////////
	//   01 02   //   01    03//      02 03//
	//   11 12   //   11    13//      12 13//
	//20       23//20    22   //20 21      //
	//30       33//30    32   //30 31      //
	/////////////////////////////////////////

	// calc minors

	// register component layout: w, z, y, x
	__m128 row0 = iMat.row[0];
	__m128 row1 = iMat.row[1];
	__m128 row2 = iMat.row[2];
	__m128 row3 = iMat.row[3];

	//////////////////////////////////////////////////////////////////////////
	//00 01      //                        //
	//10 11      //                        //
	//      22 23//                        //
	//      32 33//                        //
	/////////////////////////////////////////
	//                        //      02 03//
	//                        //      12 13//
	//                        //20 21      //
	//                        //30 31      //
	/////////////////////////////////////////
	__m128 s0s5c0c5;
	{
		__m128 tmp = _mm_shuffle_ps(row1, row1, EM_SHUFFLE(1,0,3,2));
		// 00 01 02 03
		//      *
		// 11 10 13 12        
		__m128 s05 = _mm_mul_ps(row0, tmp);
		tmp = _mm_shuffle_ps(row3, row3, EM_SHUFFLE(1,0,3,2));
		// 20 21 22 23
		//      *
		// 31 30 33 32
		__m128 c05 = _mm_mul_ps(row2, tmp);
		// SSE3
		s0s5c0c5 = _mm_hsub_ps(s05, c05);
		// (00*11 - 01*10) (02*13 - 03*12) (20*31 - 21*30) (22*33 - 23*32)
	}

	//////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////
	//           //00    02   //           //
	//           //10    12   //           //
	//           //   21    23//           //
	//           //   31    33//           //
	/////////////////////////////////////////
	//           //   01    03//           //
	//           //   11    13//           //
	//           //20    22   //           //
	//           //30    32   //           //
	/////////////////////////////////////////
	__m128 s1s4c4c1;
	{
		__m128 tmpTop = _mm_shuffle_ps(row0, row0, EM_SHUFFLE(0,2,1,3));
		__m128 tmpBot = _mm_shuffle_ps(row1, row1, EM_SHUFFLE(2,0,3,1));
		// 00 02 01 03
		//      *
		// 12 10 13 11
		__m128 s14 = _mm_mul_ps(tmpTop, tmpBot);

		tmpTop = _mm_shuffle_ps(row2, row2, EM_SHUFFLE(1,3,0,2));
		tmpBot = _mm_shuffle_ps(row3, row3, EM_SHUFFLE(3,1,2,0));
		// 21 23 20 22
		//      *
		// 33 31 32 30
		__m128 c41 = _mm_mul_ps(tmpTop, tmpBot);

		s1s4c4c1 = _mm_hsub_ps(s14, c41);
		// (00*12 - 02*10) (01*13 - 03*11) (20*32 - 21*33) (22*30 - 23*31)
	}

	//////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////
	//                        //00       03//
	//                        //10       13//
	//                        //   21 22   //
	//                        //   31 32   //
	/////////////////////////////////////////
	//   01 02   //                        //
	//   11 12   //                        //
	//20       23//                        //
	//30       33//                        //
	/////////////////////////////////////////

	__m128 s2s3c3c2;
	{

		__m128 tmpTop = _mm_shuffle_ps(row0, row0, EM_SHUFFLE(0,3,1,2));
		__m128 tmpBot = _mm_shuffle_ps(row1, row1, EM_SHUFFLE(3,0,2,1));
		// 00 03 01 02
		//      *
		// 13 10 12 11
		__m128 s23 = _mm_mul_ps(tmpTop, tmpBot);
		tmpTop = _mm_shuffle_ps(row2, row2, EM_SHUFFLE(1,2,0,3));
		tmpBot = _mm_shuffle_ps(row3, row3, EM_SHUFFLE(2,1,3,0));
		// 21 22 20 23
		//      *
		// 32 31 33 30
		__m128 c32 = _mm_mul_ps(tmpTop, tmpBot);

		s2s3c3c2 = _mm_hsub_ps(s23, c32);
		// (00*13 - 03*10) (01*12 - 02*11) (21*32 - 22*31) (20*33 - 23*30)
	}

	__m128 _s0s5s1s4 = _mm_shuffle_ps(s0s5c0c5, s1s4c4c1, EM_SHUFFLE(0,1,0,1));
	__m128 _c5c0c4c1 = _mm_shuffle_ps(s0s5c0c5, s1s4c4c1, EM_SHUFFLE(3,2,2,3));
	__m128 _c3c2c4c1 = _mm_shuffle_ps(s2s3c3c2, s1s4c4c1, EM_SHUFFLE(2,3,2,3));

	__m128 _s0c5_s5c0_s1c4_s4c1 = _mm_mul_ps(_s0s5s1s4, _c5c0c4c1);
	__m128 _s2c3_s3c2 = _mm_mul_ps(s2s3c3c2, _c3c2c4c1);
	__m128 _tmp = _mm_hadd_ps(_s0c5_s5c0_s1c4_s4c1, _s2c3_s3c2); // (s0c5 + s5c0) (s1c4 + s4c1) (s2c3 + s3c2) (?? + ??)
	__m128 _tmp2 = _mm_hsub_ps(_tmp, _tmp); // (s0c5_+_s5c0) - (s1c4_+_s4c1)
	__m128 det = _mm_add_ps( _mm_shuffle_ps(_tmp2, _tmp2, EM_SHUFFLE(0,0,0,0)), 
		_mm_shuffle_ps(_tmp, _tmp, EM_SHUFFLE(2,2,2,2))); // (s0c5_+_s5c0)_-_(s1c4_+_s4c1) + (s2c3_+_s3c2)
	__m128 _invDet = _mm_div_ps(_mm_set_ps1(1.f), det);

	// float invDet = 1.f / (s0*c5 - s1*c4 + s2*c3 + s3*c2 - s4*c1 + s5*c0);

	//
	//s0 s1 s2 s3 s4 s5               s0 s5 s1 s4 s2 s3
	//  -  +  +  -  +         ==>       +  -  -  +  +
	//c5 c4 c3 c2 c1 c0               c5 c0 c4 c1 c3 c2
	//                                              // switch sign
	//                               // (s0c5 + s5c0) - (s1c4 + s4c1) + (s2c3 + s3c2)
	//
	//                               // s0c5 s1c4 s2c3
	//                               //  +    +    +
	//                               // s5c0 s4c1 s3c2

	__m128 invDet1 = _mm_mul_ps(_invDet, _mm_set_ps(-1.f,1.f,-1.f,1.f));
	__m128 invDet2 = _mm_mul_ps(_invDet, _mm_set_ps(1.f,-1.f,1.f,-1.f));

	__m128 _c5c5c4c3 = _mm_shuffle_ps(_c5c0c4c1, _c3c2c4c1, EM_SHUFFLE(0,0,2,0));
	__m128 _c4c2c2c1 = _mm_shuffle_ps(_c3c2c4c1, _c3c2c4c1, EM_SHUFFLE(2,1,1,3));
	__m128 _c3c1c0c0 = _mm_shuffle_ps(_c3c2c4c1, _c5c0c4c1, EM_SHUFFLE(0,3,1,1));

	__m128 _adjCol0;
	{
		__m128 tmp0 = _mm_mul_ps(_c5c5c4c3,  _mm_shuffle_ps(row1, row1, EM_SHUFFLE(1,0,0,0)) );
		__m128 tmp1 = _mm_mul_ps(_c4c2c2c1,  _mm_shuffle_ps(row1, row1, EM_SHUFFLE(2,2,1,1)) );
		__m128 tmp2 = _mm_mul_ps(_c3c1c0c0,  _mm_shuffle_ps(row1, row1, EM_SHUFFLE(3,3,3,2)) );
		__m128 tmp3 = _mm_add_ps( _mm_sub_ps(tmp0, tmp1), tmp2 );
		_adjCol0 = _mm_mul_ps(tmp3, invDet1);
	}

	__m128 _adjCol1;
	{
		__m128 tmp0 = _mm_mul_ps(_c5c5c4c3,  _mm_shuffle_ps(row0, row0, EM_SHUFFLE(1,0,0,0)) );
		__m128 tmp1 = _mm_mul_ps(_c4c2c2c1,  _mm_shuffle_ps(row0, row0, EM_SHUFFLE(2,2,1,1)) );
		__m128 tmp2 = _mm_mul_ps(_c3c1c0c0,  _mm_shuffle_ps(row0, row0, EM_SHUFFLE(3,3,3,2)) );
		__m128 tmp3 = _mm_add_ps( _mm_sub_ps(tmp0, tmp1), tmp2 );
		_adjCol1 = _mm_mul_ps(tmp3, invDet2);
	}

	// bank
	__m128 _s2s3s1s4 = _mm_shuffle_ps(s2s3c3c2, _s0s5s1s4, EM_SHUFFLE(0,1,2,3));

	__m128 _s5s5s4s3 = _mm_shuffle_ps(_s0s5s1s4, _s2s3s1s4, EM_SHUFFLE(1,1,3,1));
	__m128 _s4s2s2s1 = _mm_shuffle_ps(_s2s3s1s4, _s2s3s1s4, EM_SHUFFLE(3,0,0,2));
	__m128 _s3s1s0s0 = _mm_shuffle_ps(_s2s3s1s4, _s0s5s1s4, EM_SHUFFLE(1,2,0,0));


	__m128 _adjCol2;
	{
		__m128 tmp0 = _mm_mul_ps(_s5s5s4s3,  _mm_shuffle_ps(row3, row3, EM_SHUFFLE(1,0,0,0)) );
		__m128 tmp1 = _mm_mul_ps(_s4s2s2s1,  _mm_shuffle_ps(row3, row3, EM_SHUFFLE(2,2,1,1)) );
		__m128 tmp2 = _mm_mul_ps(_s3s1s0s0,  _mm_shuffle_ps(row3, row3, EM_SHUFFLE(3,3,3,2)) );
		__m128 tmp3 = _mm_add_ps( _mm_sub_ps(tmp0, tmp1), tmp2 );
		_adjCol2 = _mm_mul_ps(tmp3, invDet1);
	}


	__m128 _adjCol3;
	{
		__m128 tmp0 = _mm_mul_ps(_s5s5s4s3,  _mm_shuffle_ps(row2, row2, EM_SHUFFLE(1,0,0,0)) );
		__m128 tmp1 = _mm_mul_ps(_s4s2s2s1,  _mm_shuffle_ps(row2, row2, EM_SHUFFLE(2,2,1,1)) );
		__m128 tmp2 = _mm_mul_ps(_s3s1s0s0,  _mm_shuffle_ps(row2, row2, EM_SHUFFLE(3,3,3,2)) );
		__m128 tmp3 = _mm_add_ps( _mm_sub_ps(tmp0, tmp1), tmp2 );
		_adjCol3 = _mm_mul_ps(tmp3, invDet2);
	}

	XMatrix result = xmatrix(_adjCol0, _adjCol1, _adjCol2, _adjCol3);
	result = transpose(result);
	return result;
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
	XVector x = splatX(iVec);
	XVector y = splatY(iVec);
	XVector z = splatZ(iVec);
	XVector w = splatW(iVec);
	XVector prod1 = _mm_mul_ps(iMat.row[0], x);
	XVector prod2 = _mm_mul_ps(iMat.row[1], y);
	XVector prod3 = _mm_mul_ps(iMat.row[2], z);
	XVector prod4 = _mm_mul_ps(iMat.row[3], w);
	XVector result = _mm_add_ps(_mm_add_ps(prod1, prod2), _mm_add_ps(prod3, prod4));
	return result;
}


//////////////////////////////////////////////////////////////////////////

inline XVector transformCoord(XMatrixParam iMat, XVectorParam iVec)
{
	XVector x = splatX(iVec);
	XVector y = splatY(iVec);
	XVector z = splatZ(iVec);
	XVector prod1 = _mm_mul_ps(iMat.row[0], x);
	XVector prod2 = _mm_mul_ps(iMat.row[1], y);
	XVector prod3 = _mm_mul_ps(iMat.row[2], z);
	XVector result = _mm_add_ps(_mm_add_ps(prod1, prod2), _mm_add_ps(prod3, iMat.row[3]));
	XVector w = splatW(result);
	XVector recipW = _mm_div_ps(xvector(1.f), w);
	result = _mm_mul_ps(result, recipW);
	return result;
}


//////////////////////////////////////////////////////////////////////////

inline XVector transformNormal(XMatrixParam iMat, XVectorParam iVec)
{
	XVector x = splatX(iVec);
	XVector y = splatY(iVec);
	XVector z = splatZ(iVec);
	XVector prod1 = _mm_mul_ps(iMat.row[0], x);
	XVector prod2 = _mm_mul_ps(iMat.row[1], y);
	XVector prod3 = _mm_mul_ps(iMat.row[2], z);
	XVector result = _mm_add_ps(_mm_add_ps(prod1, prod2), prod3);
	return result;
}


//////////////////////////////////////////////////////////////////////////

inline void transformArray(XMatrixParam iMat, XVector* ioArray, uint iCount)
{
	for(uint vecIter = 0; vecIter < iCount; ++vecIter)
	{
		XVector vec = ioArray[vecIter];

		XVector x = splatX(vec);
		XVector y = splatY(vec);
		XVector z = splatZ(vec);
		XVector w = splatW(vec);
		XVector prod1 = _mm_mul_ps(iMat.row[0], x);
		XVector prod2 = _mm_mul_ps(iMat.row[1], y);
		XVector prod3 = _mm_mul_ps(iMat.row[2], z);
		XVector prod4 = _mm_mul_ps(iMat.row[3], w);
		ioArray[vecIter] = _mm_add_ps(_mm_add_ps(prod1, prod2), _mm_add_ps(prod3, prod4));
	}
}

//////////////////////////////////////////////////////////////////////////

inline void transformCoordArray(XMatrixParam iMat, XVector* ioArray, uint iCount)
{
	xvector kOne = xvector(1.f);
	for(uint vecIter = 0; vecIter < iCount; ++vecIter)
	{
		XVector vec = ioArray[vecIter];

		XVector x = splatX(vec);
		XVector y = splatY(vec);
		XVector z = splatZ(vec);
		XVector prod1 = _mm_mul_ps(iMat.row[0], x);
		XVector prod2 = _mm_mul_ps(iMat.row[1], y);
		XVector prod3 = _mm_mul_ps(iMat.row[2], z);

		XVector result = _mm_add_ps(_mm_add_ps(prod1, prod2), _mm_add_ps(prod3, iMat.row[3]));
		XVector w = splatW(result);
		XVector recipW = _mm_div_ps(kOne, w);
		ioArray[vecIter] = _mm_mul_ps(result, recipW);
	}
}

//////////////////////////////////////////////////////////////////////////

inline void transformVectorArray(XMatrixParam iMat, XVector* ioArray, uint iCount)
{
	for(uint vecIter = 0; vecIter < iCount; ++vecIter)
	{
		XVector vec = ioArray[vecIter];

		XVector x = splatX(vec);
		XVector y = splatY(vec);
		XVector z = splatZ(vec);
		XVector prod1 = _mm_mul_ps(iMat.row[0], x);
		XVector prod2 = _mm_mul_ps(iMat.row[1], y);
		XVector prod3 = _mm_mul_ps(iMat.row[2], z);
		ioArray[vecIter] = _mm_add_ps(_mm_add_ps(prod1, prod2), prod3);
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
