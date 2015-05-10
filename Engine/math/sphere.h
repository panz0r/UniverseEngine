#pragma once

#include "math_types.h"
#include "vector3.h"

namespace ue
{

inline Sphere sphere();
inline Sphere sphere(Vector3Param iCenter, float iRadius);
inline Sphere sphere(float iX, float iY, float iZ, float iRadius);

inline bool intersectSphereRay(SphereParam iSphere, RayParam iRay, float& oIntersectPoint0, float& oIntersectPoint1);
inline bool intersectSphereSphere(SphereParam iSphereA, SphereParam iSphereB, float& oIntersectAreaA);
inline bool intersectSphereSphere(SphereParam iSphereA, SphereParam iSphereB, Vector3& oCapCentroid, float& oCapRadius, float& oCapArea);

//inline bool intersectSphereRay(SphereParam iSphere, RayParam iRay, Vector3& oIntersectPoint0, Vector3& oIntersectPoint1);
//inline bool intersectSphereRay(SphereParam iSphere, RayParam iRay, float iMaxLength, Vector3& oIntersectPoint0, Vector3& oIntersectPoint1);

#include "sphere.inl"

}