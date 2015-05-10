inline XRay xray(XVectorParam iOrigin, XVectorParam iDirection)
{
	XRay ret;
	ret.origin = iOrigin;
	ret.direction = iDirection;
	return ret;
}