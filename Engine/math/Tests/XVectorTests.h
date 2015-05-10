#pragma once

namespace em
{
namespace XVectorTests
{
	bool createFromPtrTest();
	bool createFromComponentsTest();
	bool createFromScalarTest();

	bool equalTest();
	bool aproxEqualTest();
	bool lessTest();
	bool lessEqualTest();
	bool greaterTest();
	bool greaterEqualTest();
	

	bool negateTest();
	bool addTest();
	bool subTest();
	bool mulTest();
	bool divTest();

	bool getXTest();
	bool getYTest();
	bool getZTest();
	bool getWTest();
	bool getComponentTest();

	bool dotTest();
	bool lengthSqTest();
	bool distanceSqTest();
	bool lengthTest();
	bool distanceTest();
	bool normalizeTest();
	bool crossTest();

	bool splatXTest();
	bool splatYTest();
	bool splatZTest();
	bool splatWTest();
}

}
