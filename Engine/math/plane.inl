
inline Plane plane(Vector3Param iNormal, float iD)
{
	Plane ret = { iNormal, iD };
	return ret;
}

inline bool intersectPlaneRay(PlaneParam iPlane, RayParam iRay, float &oIntersectPoint)
{
	float vn = dot(iRay.direction, iPlane.normal);
	if(vn >= 0)
		return false;

	oIntersectPoint = -(dot(iRay.origin, iPlane.normal) + iPlane.d) / vn;
	return true;
}
