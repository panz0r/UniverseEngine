
inline XPlane xplane(XVectorParam iNormal, XVectorParam iD)
{
	XPlane ret;
	ret.normal = iNormal;
	ret.d = iD;
	return ret;
}

inline bool intersectPlaneRay(XPlaneParam iPlane, XRayParam iRay, XVector &oIntersectDistance)
{
	static XVector xzero = xvector(0);

	XVector vn = dotVector(iRay.direction, iPlane.normal);
	if(greaterEqual(vn, xzero))
		return false;

	oIntersectDistance = div( negate( add(dotVector(iRay.origin, iPlane.normal), iPlane.d) ), vn);
	return true;
}
