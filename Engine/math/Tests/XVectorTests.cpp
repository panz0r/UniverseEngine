#include "Pch.h"
#include "XVectorTests.h"
#include <Core/Math/emXVector.h>
#include <Core/Math/emVector3.h>
#include <Core/Math/emVector4.h>

namespace em
{
namespace XVectorTests
{
	const float EM_DECL_ALIGN(16) kVec1[] = {3.4f, 200.67f, 9.8f, 1.1f};
	const float EM_DECL_ALIGN(16) kVec2[] = {8.4f, 0.2f, 4.272f, 0.0021f};
	const float EM_DECL_ALIGN(16) kCrossVec1[] = {0.f, 3.f, 0.4f, 0.0f};
	const float EM_DECL_ALIGN(16) kCrossVec2[] = {1.f, 0.0f, -0.3f, 0.0f};
	const float EM_DECL_ALIGN(16) kNegVec1[] = {-kVec1[0], -kVec1[1], -kVec1[2], -kVec1[3]};
	const float EM_DECL_ALIGN(16) kSum[] = {kVec1[0]+kVec2[0], kVec1[1]+kVec2[1], kVec1[2]+kVec2[2], kVec1[3]+kVec2[3] };
	const float EM_DECL_ALIGN(16) kDiff[] = {kVec1[0]-kVec2[0], kVec1[1]-kVec2[1], kVec1[2]-kVec2[2], kVec1[3]-kVec2[3] };
	const float EM_DECL_ALIGN(16) kProd[] = {kVec1[0]*kVec2[0], kVec1[1]*kVec2[1], kVec1[2]*kVec2[2], kVec1[3]*kVec2[3] };
	const float EM_DECL_ALIGN(16) kQuot[] = {kVec1[0]/kVec2[0], kVec1[1]/kVec2[1], kVec1[2]/kVec2[2], kVec1[3]/kVec2[3] };
	const float kDot11 = { kVec1[0]*kVec1[0] + kVec1[1]*kVec1[1] + kVec1[2]*kVec1[2] + kVec1[3]*kVec1[3] };
	const float kDot12 = { kVec1[0]*kVec2[0] + kVec1[1]*kVec2[1] + kVec1[2]*kVec2[2] + kVec1[3]*kVec2[3] };
	const float kOneOverSqrtDot11 = 1.f / sqrtf(kDot11);
	const float EM_DECL_ALIGN(16) kVec1norm[] = {kVec1[0] * kOneOverSqrtDot11, kVec1[1] * kOneOverSqrtDot11, kVec1[2] * kOneOverSqrtDot11, kVec1[3] * kOneOverSqrtDot11 };

	// assumes first test passes
	const XVector vec1 = xvector(kVec1);
	const XVector vec2 = xvector(kVec2);
	
	bool createFromPtrTest()
	{
		XVector vec = xvector(kVec1);
		EM_TEST_VERIFY(getX(vec) == kVec1[0] && 
			getY(vec) == kVec1[1] && 
			getZ(vec) == kVec1[2] && 
			getW(vec) == kVec1[3]);
		return true;
	}

	bool createFromComponentsTest()
	{
		XVector vec = xvector(kVec1[0], kVec1[1], kVec1[2], kVec1[3]);
		EM_TEST_VERIFY(getX(vec) == kVec1[0] && 
			getY(vec) == kVec1[1] && 
			getZ(vec) == kVec1[2] && 
			getW(vec) == kVec1[3]);
		return true;	
	}

	bool createFromScalarTest()
	{
		XVector vec = xvector(kDot11);
		EM_TEST_VERIFY(getX(vec) == kDot11 &&
			getY(vec) == kDot11 &&
			getZ(vec) == kDot11 &&
			getW(vec) == kDot11);
		return true;
	}

	bool equalTest()
	{
		EM_TEST_VERIFY(!equal(vec1, vec2));
		EM_TEST_VERIFY(equal(vec1, vec1));
		EM_TEST_VERIFY(equal(vec2, vec2));
		return true;
	}

	bool lessTest()
	{
		// scalar test
		EM_TEST_VERIFY(less(xvector(0.3f), xvector(0.31f)));
		EM_TEST_VERIFY(!less(xvector(2.3f), xvector(0.1f)));
		EM_TEST_VERIFY(!less(xvector(2.23f), xvector(2.23f)));

		// only x component should matter
		EM_TEST_VERIFY(less(xvector(0.3f, 0.2f, 0.6f, 0.3f), xvector(0.31f, 0.99f, 0.01f, 0.1f)));
		EM_TEST_VERIFY(!less(xvector(2.3f, 0.1f, 0.6f, 0.3f), xvector(0.1f, 0.99f, 0.01f, 0.1f)));
		EM_TEST_VERIFY(!less(xvector(2.3f, 0.1f, 0.6f, 0.3f), xvector(2.3f, 0.99f, 0.01f, 0.1f)));
		return true;
	}

	bool lessEqualTest()
	{
		// scalar test
		EM_TEST_VERIFY(lessEqual(xvector(0.3f), xvector(0.31f)));
		EM_TEST_VERIFY(!lessEqual(xvector(2.3f), xvector(0.1f)));
		EM_TEST_VERIFY(lessEqual(xvector(2.23f), xvector(2.23f)));

		// only x component should matter
		EM_TEST_VERIFY(lessEqual(xvector(0.3f, 0.2f, 0.6f, 0.3f), xvector(0.31f, 0.99f, 0.01f, 0.1f)));
		EM_TEST_VERIFY(!lessEqual(xvector(2.3f, 0.1f, 0.6f, 0.3f), xvector(0.1f, 0.99f, 0.01f, 0.1f)));
		EM_TEST_VERIFY(lessEqual(xvector(2.3f, 0.1f, 0.6f, 0.3f), xvector(2.3f, 0.99f, 0.01f, 0.1f)));
		return true;
	}

	bool greaterTest()
	{
		// scalar test
		EM_TEST_VERIFY(!greater(xvector(0.3f), xvector(0.31f)));
		EM_TEST_VERIFY(greater(xvector(2.3f), xvector(0.1f)));
		EM_TEST_VERIFY(!greater(xvector(2.23f), xvector(2.23f)));

		// only x component should matter
		EM_TEST_VERIFY(!greater(xvector(0.3f, 0.2f, 0.6f, 0.3f), xvector(0.31f, 0.99f, 0.01f, 0.1f)));
		EM_TEST_VERIFY(greater(xvector(2.3f, 0.1f, 0.6f, 0.3f), xvector(0.1f, 0.99f, 0.01f, 0.1f)));
		EM_TEST_VERIFY(!greater(xvector(2.3f, 0.1f, 0.6f, 0.3f), xvector(2.3f, 0.99f, 0.01f, 0.1f)));
		return true;
	}

	bool greaterEqualTest()
	{
		// scalar test
		EM_TEST_VERIFY(!greaterEqual(xvector(0.3f), xvector(0.31f)));
		EM_TEST_VERIFY(greaterEqual(xvector(2.3f), xvector(0.1f)));
		EM_TEST_VERIFY(greaterEqual(xvector(2.23f), xvector(2.23f)));

		// only x component should matter
		EM_TEST_VERIFY(!greaterEqual(xvector(0.3f, 0.2f, 0.6f, 0.3f), xvector(0.31f, 0.99f, 0.01f, 0.1f)));
		EM_TEST_VERIFY(greaterEqual(xvector(2.3f, 0.1f, 0.6f, 0.3f), xvector(0.1f, 0.99f, 0.01f, 0.1f)));
		EM_TEST_VERIFY(greaterEqual(xvector(2.3f, 0.1f, 0.6f, 0.3f), xvector(2.3f, 0.99f, 0.01f, 0.1f)));
		return true;
	}


	bool aproxEqualTest()
	{
		//EM_TEST_VERIFY(!aproxEqual(vec1, vec2));
		//EM_TEST_VERIFY(aproxEqual(vec1, vec1));
		//EM_TEST_VERIFY(aproxEqual(vec2, vec2));
		return true;
	}

	bool negateTest()
	{
		EM_TEST_VERIFY(equal(negate(vec1), xvector(kNegVec1)));
		return true;
	}

	bool addTest()
	{
		EM_TEST_VERIFY(equal(add(vec1, vec2), xvector(kSum)));
		return true;
	}

	bool subTest()
	{
		EM_TEST_VERIFY(equal(sub(vec1, vec2), xvector(kDiff)));
		return true;
	}

	bool mulTest()
	{
		EM_TEST_VERIFY(equal(mul(vec1, vec2), xvector(kProd)));
		return true;
	}

	bool divTest()
	{
		EM_TEST_VERIFY(equal(div(vec1, vec2), xvector(kQuot)));
		return true;
	}

	bool getXTest()
	{
		EM_TEST_VERIFY(getX(vec1) == kVec1[0]);
		return true;
	}

	bool getYTest()
	{
		EM_TEST_VERIFY(getY(vec1) == kVec1[1]);
		return true;
	}

	bool getZTest()
	{
		EM_TEST_VERIFY(getZ(vec1) == kVec1[2]);
		return true;
	}

	bool getWTest()
	{
		EM_TEST_VERIFY(getW(vec1) == kVec1[3]);
		return true;
	}
	
	bool getComponentTest()
	{
		EM_TEST_VERIFY(getComponent(vec1, 0) == kVec1[0] &&
			getComponent(vec1, 1) == kVec1[1] &&
			getComponent(vec1, 2) == kVec1[2] &&
			getComponent(vec1, 3) == kVec1[3]);
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
		XVector testVec = normalize(vec1);
		EM_TEST_VERIFY(aproxEqual(testVec, xvector(kVec1norm)));
		return true;
	}

	bool crossTest()
	{
		Vector3 ref = cross(vector3(kCrossVec1), vector3(kCrossVec2));
		XVector vec = cross(xvector(kCrossVec1), xvector(kCrossVec2));
		EM_TEST_VERIFY(getX(vec) == ref.x &&
			getY(vec) == ref.y &&
			getZ(vec) == ref.z &&
			getW(vec) == 0.0f);
		return true;
	}

	bool splatXTest()
	{
		XVector testVec = splatX(vec1);
		EM_TEST_VERIFY(getX(testVec) == kVec1[0] &&
			getY(testVec) == kVec1[0] &&
			getZ(testVec) == kVec1[0] &&
			getW(testVec) == kVec1[0]);
		return true;
	}

	bool splatYTest()
	{
		XVector testVec = splatY(vec1);
		EM_TEST_VERIFY(getX(testVec) == kVec1[1] &&
			getY(testVec) == kVec1[1] &&
			getZ(testVec) == kVec1[1] &&
			getW(testVec) == kVec1[1]);
		return true;
	}

	bool splatZTest()
	{
		XVector testVec = splatZ(vec1);
		EM_TEST_VERIFY(getX(testVec) == kVec1[2] &&
			getY(testVec) == kVec1[2] &&
			getZ(testVec) == kVec1[2] &&
			getW(testVec) == kVec1[2]);
		return true;
	}

	bool splatWTest()
	{
		XVector testVec = splatW(vec1);
		EM_TEST_VERIFY(getX(testVec) == kVec1[3] &&
			getY(testVec) == kVec1[3] &&
			getZ(testVec) == kVec1[3] &&
			getW(testVec) == kVec1[3]);
		return true;
	}
}

}