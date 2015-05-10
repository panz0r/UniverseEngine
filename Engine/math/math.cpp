#include "math.h"

#include <limits>

namespace ue
{

//////////////////////////////////////////////////////////////////////////

const float Math::kPI = 3.1415926535897f;
const float Math::kRAD_TO_DEG = 180.f / Math::kPI;
const float Math::kDEG_TO_RAD = Math::kPI / 180.f;
const float Math::kFLOAT_MIN = std::numeric_limits<float>::min();
const float Math::kFLOAT_MAX = std::numeric_limits<float>::max();
const int Math::kINT_MIN = std::numeric_limits<int>::min();
const int Math::kINT_MAX = std::numeric_limits<int>::max();
const float Math::kFLOAT_EPS = std::numeric_limits<float>::epsilon();

//////////////////////////////////////////////////////////////////////////


} // namespace em
