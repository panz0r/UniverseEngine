
inline Ray ray(Vector3Param iOrigin, Vector3Param iDirection)
{
	Ray ret;
	ret.origin = iOrigin;
	ret.direction = iDirection;
	return ret;
}

//inline Ray ray(Vector3Param iStart, Vector3Param iEnd)
//{
//	Ray ret;
//	ret.origin = iStart;
//	ret.direction = normalize(iEnd - iStart);
//	return ret;
//}
