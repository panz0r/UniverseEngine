#pragma once

#include "math_types.h"
#include "vector3.h"

namespace ue
{

Plane plane(Vector3Param iNormal, float iD);
bool intersectPlaneRay(PlaneParam iPlane, RayParam iRay, float &oIntersectPoint);

#include "plane.inl"

}