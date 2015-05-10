#pragma once

#include <Core/Platform/emTypedefs.h>

namespace ue
{
	EM_DECL_ALIGN(16) struct XVector
	{
		union
		{
			struct
			{
				float x,y,z,w;
			};
			float v[4];
		};
	};

	typedef const XVector& XVectorParam;


	typedef XVector XQuaternion;
	typedef const XQuaternion& XQuaternionParam;


	EM_DECL_ALIGN(16) struct XMatrix
	{
		union
		{
			XVector row[4];
			struct // for internal use only!
			{
				float m00,m01,m02,m03;
				float m10,m11,m12,m13;
				float m20,m21,m22,m23;
				float m30,m31,m32,m33;
			};

		}
	};

	typedef const XMatrix& XMatrixParam;

	
}