#pragma once

namespace em
{
namespace Vector4Tests
{


bool createFromPtrTest();
bool createFromComponentsTest();
bool createFromScalarTest();

bool equalTest();
bool aproxEqualTest();

bool negateTest();
bool addTest();
bool subTest();
bool mulTest();
bool divTest();

bool getComponentTest();

bool dotTest();
bool lengthSqTest();
bool distanceSqTest();
bool lengthTest();
bool distanceTest();
bool normalizeTest();

bool splatXTest();
bool splatYTest();
bool splatZTest();
bool splatWTest();

}
}