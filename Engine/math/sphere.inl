
inline Sphere sphere()
{
	Sphere ret = {0};
	return ret;
}

inline Sphere sphere(Vector3Param iCenter, float iRadius)
{
	Sphere ret;
	ret.center = iCenter;
	ret.radius = iRadius;
	ret.radiusSq = iRadius*iRadius;
	return ret;
}

inline Sphere sphere(float iX, float iY, float iZ, float iRadius)
{
	Sphere ret;
	ret.center = vector3(iX,iY,iZ);
	ret.radius = iRadius;
	ret.radiusSq = iRadius*iRadius;
	return ret;
}

inline bool intersectSphereRay(SphereParam iSphere, RayParam iRay, float& oIntersectPoint0, float& oIntersectPoint1)
{

	Vector3 l = iSphere.center - iRay.origin;
	float tca = dot(l, iRay.direction);
	
	float distSq = dot(l,l);
	if(tca < 0)
	{
		return false;
	}
	

	float d2 = distSq - tca*tca;
	
	if(d2 > iSphere.radiusSq)
		return false;

	float thc = sqrtf(iSphere.radiusSq - d2);

	oIntersectPoint0 = tca - thc;
	oIntersectPoint1 = tca + thc;
	return true;
}

inline bool intersectSphereSphere(SphereParam iSphereA, SphereParam iSphereB, float& oIntersectAreaA)
{
	float d = length(iSphereA.center - iSphereB.center);

	float d2 = d*d;
	float x = (d2 - iSphereB.radiusSq + iSphereA.radiusSq) / (d*2.f);
	if (x > iSphereA.radius)
		return false;

	// calc area of intersection cap
	//float r2 = iSphereA.radiusSq - x*x;	// Pythagorean theorem to get the area2 of the intersection circle
	float h = iSphereA.radius - x;
	oIntersectAreaA = 2.f * Math::kPI * iSphereA.radius * h;
	return true;
}


inline bool intersectSphereSphere(SphereParam iSphereA, SphereParam iSphereB, Vector3& oCapCentroid, float& oCapRadius, float& oCapArea)
{
	float d = 0;
	Vector3 capDirectionVec = normalize(iSphereB.center - iSphereA.center, d);
	
	float d2 = d*d;
	float x = (d2 - iSphereB.radiusSq + iSphereA.radiusSq) / (d*2.f);
	if (x > iSphereA.radius)
		return false;

	oCapCentroid = iSphereA.center + capDirectionVec * x;

	// calc area of intersection cap
	float r2 = iSphereA.radiusSq - x*x;	// Pythagorean theorem to get the radius2 of the intersection circle
	oCapRadius = sqrtf(r2);

	float h = iSphereA.radius - x;
	oCapArea = 2.f * Math::kPI * iSphereA.radius * h;
	return true;
}

inline bool intersectSpherePlane(SphereParam iSphere, PlaneParam iPlane, float& oIntersectArea)
{
	
}