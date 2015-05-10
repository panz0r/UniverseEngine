#pragma once

#include "math.h"
#include "math_types.h"
#include "vector3.h"

namespace ue
{

Ray ray(Vector3Param iOrigin, Vector3Param iDirection);
//Ray ray(Vector3Param iStart, Vector3Param iEnd);

#include "ray.inl"

}