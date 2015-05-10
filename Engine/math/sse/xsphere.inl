
inline XSphere xsphere()
{
	Sphere ret = {0};
	return ret;
}

inline XSphere xsphere(XVectorParam iCenter, XVectorParam iRadius)
{
	XSphere ret;
	ret.center = iCenter;
	ret.radiusSq = mul(iRadius, iRadius);
	return ret;
}

inline XSphere xsphere(float iX, float iY, float iZ, float iRadius)
{
	XSphere ret;
	ret.center = xvector(iX,iY,iZ, 0);
	ret.radiusSq = xvector(iRadius*iRadius);
	return ret;
}

inline bool intersectSphereRay(XSphereParam iSphere, XRayParam iRay, XVector& oIntersectDistance0, XVector& oIntersectDistance1)
{
	static XVector xzero = xvector(0);

	XVector l = sub(iSphere.center, iRay.origin);
	XVector tca = dotVector(l, iRay.direction);

	if(less(tca, xzero))
	{
		return false;
	}

	XVector distSq = dotVector(l,l);
	XVector d2 = distSq - mul(tca,tca);

	if(greater(d2, iSphere.radiusSq))
		return false;

	XVector thc = sqrt(iSphere.radiusSq - d2);

	oIntersectDistance0 = tca - thc;
	oIntersectDistance1 = tca + thc;
	return true;
}