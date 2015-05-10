#include "Pch.h"
#include "Vector4Tests.h"
#include <Core/Math/emVector4.h>


namespace em
{
namespace Vector4Tests
{

const float kVec1[] = {3.4f, 200.67f, 9.8f, 1.1f};
const float kVec2[] = {8.4f, 0.2f, 4.272f, 0.0021f};
const float kNegVec1[] = {-kVec1[0], -kVec1[1], -kVec1[2], -kVec1[3]};
const float kSum[] = {kVec1[0]+kVec2[0], kVec1[1]+kVec2[1], kVec1[2]+kVec2[2], kVec1[3]+kVec2[3] };
const float kDiff[] = {kVec1[0]-kVec2[0], kVec1[1]-kVec2[1], kVec1[2]-kVec2[2], kVec1[3]-kVec2[3] };
const float kProd[] = {kVec1[0]*kVec2[0], kVec1[1]*kVec2[1], kVec1[2]*kVec2[2], kVec1[3]*kVec2[3] };
const float kQuot[] = {kVec1[0]/kVec2[0], kVec1[1]/kVec2[1], kVec1[2]/kVec2[2], kVec1[3]/kVec2[3] };
const float kDot11 = { kVec1[0]*kVec1[0] + kVec1[1]*kVec1[1] + kVec1[2]*kVec1[2] + kVec1[3]*kVec1[3] };
const float kDot12 = { kVec1[0]*kVec2[0] + kVec1[1]*kVec2[1] + kVec1[2]*kVec2[2] + kVec1[3]*kVec2[3] };
const float kVec1norm[] = {kVec1[0] / sqrtf(kDot11), kVec1[1] / sqrtf(kDot11), kVec1[2] / sqrtf(kDot11), kVec1[3] / sqrtf(kDot11) };

// assumes first test passes
const Vector4 vec1 = vector4(kVec1);
const Vector4 vec2 = vector4(kVec2);


bool createFromPtrTest()
{
	Vector4 vec = vector4(kVec1);
	EM_TEST_VERIFY(vec.x == kVec1[0] && 
		vec.y == kVec1[1] && 
		vec.z == kVec1[2] && 
		vec.w == kVec1[3]);
	return true;
}

bool createFromComponentsTest()
{
	Vector4 vec = vector4(kVec1[0], kVec1[1], kVec1[2], kVec1[3]);
	EM_TEST_VERIFY(vec.x == kVec1[0] && 
		vec.y == kVec1[1] && 
		vec.z == kVec1[2] && 
		vec.w == kVec1[3]);
	return true;	
}

bool createFromScalarTest()
{
	Vector4 vec = vector4(kDot11);
	EM_TEST_VERIFY(vec.x == kDot11 &&
		vec.y == kDot11 &&
		vec.z == kDot11 &&
		vec.w == kDot11);
	return true;
}

bool equalTest()
{
	EM_TEST_VERIFY(!equal(vec1, vec2));
	EM_TEST_VERIFY(equal(vec1, vec1));
	EM_TEST_VERIFY(equal(vec2, vec2));
	return true;
}

bool aproxEqualTest()
{
	EM_TEST_VERIFY(!aproxEqual(vec1, vec2));
	EM_TEST_VERIFY(aproxEqual(vec1, vec1));
	EM_TEST_VERIFY(aproxEqual(vec2, vec2));
	return true;
}

bool negateTest()
{
	EM_TEST_VERIFY(equal(negate(vec1), vector4(kNegVec1)));
	return true;
}

bool addTest()
{
	EM_TEST_VERIFY(equal(add(vec1, vec2), vector4(kSum)));
	return true;
}

bool subTest()
{
	EM_TEST_VERIFY(equal(sub(vec1, vec2), vector4(kDiff)));
	return true;
}

bool mulTest()
{
	EM_TEST_VERIFY(equal(mul(vec1, vec2), vector4(kProd)));
	return true;
}

bool divTest()
{
	EM_TEST_VERIFY(equal(div(vec1, vec2), vector4(kQuot)));
	return true;
}

bool getComponentTest()
{
	EM_TEST_VERIFY(getComponent(vec1, 0) == vec1.x &&
		getComponent(vec1, 1) == vec1.y &&
		getComponent(vec1, 2) == vec1.z &&
		getComponent(vec1, 3) == vec1.w);
	return true;
}

bool dotTest()
{
	EM_TEST_VERIFY(dot(vec1, vec2) == kDot12);
	return true;
}

bool lengthSqTest()
{
	EM_TEST_VERIFY(lengthSq(vec1) == kDot11);
	return true;
}

bool distanceSqTest()
{
	EM_TEST_VERIFY(distanceSq(vec1, vec2) == kDot12);
	return true;
}

bool lengthTest()
{
	EM_TEST_VERIFY(length(vec1) == sqrtf(kDot11));
	return true;
}

bool distanceTest()
{
	EM_TEST_VERIFY(distance(vec1, vec2) == sqrtf(kDot12));
	return true;
}

bool normalizeTest()
{
	Vector4 testVec = normalize(vec1);
	EM_TEST_VERIFY(aproxEqual(testVec, vector4(kVec1norm)));
	return true;
}

bool splatXTest()
{
	Vector4 testVec = splatX(vec1);
	EM_TEST_VERIFY(testVec.x == vec1.x &&
		testVec.y == vec1.x &&
		testVec.z == vec1.x &&
		testVec.w == vec1.x);
	return true;
}

bool splatYTest()
{
	Vector4 testVec = splatY(vec1);
	EM_TEST_VERIFY(testVec.x == vec1.y &&
		testVec.y == vec1.y &&
		testVec.z == vec1.y &&
		testVec.w == vec1.y);
	return true;
}

bool splatZTest()
{
	Vector4 testVec = splatZ(vec1);
	EM_TEST_VERIFY(testVec.x == vec1.z &&
		testVec.y == vec1.z &&
		testVec.z == vec1.z &&
		testVec.w == vec1.z);
	return true;
}

bool splatWTest()
{
	Vector4 testVec = splatW(vec1);
	EM_TEST_VERIFY(testVec.x == vec1.w &&
		testVec.y == vec1.w &&
		testVec.z == vec1.w &&
		testVec.w == vec1.w);
	return true;
}

}
}