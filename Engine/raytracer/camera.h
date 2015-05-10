#pragma once

#include <math/math_types.h>

namespace ue {

class Camera 
{
public:
	Camera() {}
	Camera(const Camera& iCamera);
	Camera(unsigned iWidth, unsigned iHeight, float iFov, const Matrix4x4& iLookAt);
	virtual ~Camera();

	virtual void GenerateRay(float iSampleX, float iSampleY, Ray* oRay);

//private:
	Matrix4x4 mCameraToProj;
	Matrix4x4 mRasterToProj;
	Matrix4x4 mRasterToCamera;
	Matrix4x4 mCameraToWorld;
};

}

