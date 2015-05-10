

#include "camera.h"
#include <math/matrix3.h>
#include <math/matrix4.h>
#include <math/vector3.h>
#include <math/vector4.h>


namespace ue { 

Camera::Camera(const Camera& iCamera)
: mCameraToProj(iCamera.mCameraToProj)
, mRasterToCamera(iCamera.mRasterToCamera)
, mCameraToWorld(iCamera.mCameraToWorld)
{
}

Camera::Camera(unsigned iWidth, unsigned iHeight, float iFov, const Matrix4x4& iLookAt)
{
	mCameraToProj = perspectiveFovRH(iFov, iWidth/(float)iHeight, 1e-2f, 1000.f);

	Matrix4x4 projToRaster = matrix4x4(scaling(0.5f, -0.5f, 1.f)) * 
							 translation(0.5f, 0.5f, 0.f) * 
							 matrix4x4(scaling((float)iWidth, (float)iHeight, 1));
							 
	mRasterToProj = inverse(projToRaster);
	mRasterToCamera = mRasterToProj * inverse(mCameraToProj);
	mCameraToWorld = inverse(iLookAt);
}

Camera::~Camera()
{

}


void Camera::GenerateRay(float iSampleX, float iSampleY, Ray* oRay)
{
	Vector4 rayDir = vector4(iSampleX, iSampleY, 0.f, 1.f);
	rayDir = transformCoord(mRasterToCamera, rayDir);

	oRay->origin = transformCoord(mCameraToWorld, Vector3::kZero);
	oRay->direction = normalize(transformVector(mCameraToWorld, vector3(rayDir)));
}


}

