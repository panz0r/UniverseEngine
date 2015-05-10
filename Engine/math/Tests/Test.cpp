#include "Pch.h"
#include "Vector4Tests.h"
#include "XVectorTests.h"

#include <iostream>

#include <Raytracer/emRaytracer.h>
#include <vector>

#include <Core/Math/emVector3.h>


//// testsse.cpp
//#define EM_SIMD_IMPL EM_SIMD_SSE
//#include <emMath.h> ...
//
//#include <TestVector.cxx>

#define EM_TEST(TestFunc) { \
if ((TestFunc) == true) \
	std::cout << "passed: " << #TestFunc << std::endl; \
else \
	std::cout << "failed: " << #TestFunc << std::endl; }

extern "C"
{
	__declspec(dllexport) void runTests()
	{
		using namespace em;

		EM_TEST(Vector4Tests::createFromPtrTest());
		EM_TEST(Vector4Tests::createFromComponentsTest());
		EM_TEST(Vector4Tests::createFromScalarTest());
		EM_TEST(Vector4Tests::equalTest());
		EM_TEST(Vector4Tests::aproxEqualTest());
		EM_TEST(Vector4Tests::negateTest());
		EM_TEST(Vector4Tests::addTest());
		EM_TEST(Vector4Tests::subTest());
		EM_TEST(Vector4Tests::mulTest());
		EM_TEST(Vector4Tests::divTest());
		EM_TEST(Vector4Tests::getComponentTest());
		EM_TEST(Vector4Tests::dotTest());
		EM_TEST(Vector4Tests::lengthSqTest());
		EM_TEST(Vector4Tests::distanceSqTest());
		EM_TEST(Vector4Tests::lengthTest());
		EM_TEST(Vector4Tests::distanceTest());
		EM_TEST(Vector4Tests::normalizeTest());
		EM_TEST(Vector4Tests::splatXTest());
		EM_TEST(Vector4Tests::splatYTest());
		EM_TEST(Vector4Tests::splatZTest());
		EM_TEST(Vector4Tests::splatWTest());

		// SSE
		EM_TEST(XVectorTests::createFromPtrTest());
		EM_TEST(XVectorTests::createFromComponentsTest());
		EM_TEST(XVectorTests::createFromScalarTest());
		EM_TEST(XVectorTests::equalTest());
		EM_TEST(XVectorTests::aproxEqualTest());
		EM_TEST(XVectorTests::lessTest());
		EM_TEST(XVectorTests::lessEqualTest());
		EM_TEST(XVectorTests::greaterTest());
		EM_TEST(XVectorTests::greaterEqualTest());
		EM_TEST(XVectorTests::negateTest());
		EM_TEST(XVectorTests::addTest());
		EM_TEST(XVectorTests::subTest());
		EM_TEST(XVectorTests::mulTest());
		EM_TEST(XVectorTests::divTest());
		EM_TEST(XVectorTests::getXTest());
		EM_TEST(XVectorTests::getYTest());
		EM_TEST(XVectorTests::getZTest());
		EM_TEST(XVectorTests::getWTest());
		EM_TEST(XVectorTests::getComponentTest());
		EM_TEST(XVectorTests::dotTest());
		EM_TEST(XVectorTests::lengthSqTest());
		EM_TEST(XVectorTests::distanceSqTest());
		EM_TEST(XVectorTests::lengthTest());
		EM_TEST(XVectorTests::distanceTest());
		EM_TEST(XVectorTests::normalizeTest());
		EM_TEST(XVectorTests::crossTest());
		EM_TEST(XVectorTests::splatXTest());
		EM_TEST(XVectorTests::splatYTest());
		EM_TEST(XVectorTests::splatZTest());
		EM_TEST(XVectorTests::splatWTest());
	}
};
