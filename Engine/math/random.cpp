
#include "random.h"

namespace ue
{

Random::Random()
: mSeed(23257132)
{       
	mQuadSeed = _mm_set_epi32(mSeed, mSeed+1, mSeed, mSeed+1);
}

Random::Random(int iSeed)
: mSeed(iSeed)
{
	mQuadSeed = _mm_set_epi32(iSeed, iSeed+1, iSeed, iSeed+1);
}

Random::~Random()
{
}

}