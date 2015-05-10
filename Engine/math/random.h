#include "math.h"
#include "sse/sse.h"


namespace ue
{

class Random
{
public:
	static const int kRandMax = 0x7fff;

	Random();
	Random(int iSeed);
	~Random();

	inline void srand(int iSeed);
	inline int rand();
	inline float frand();

	inline void randQuad(unsigned* oAlignedIntQuad);
	inline void frandQuad(float* oAlignedFloatQuad);

private:
	inline void randQuadInternal();

	__m128i mQuadSeed;
	int mSeed;
	
};

#include "random.inl"

}