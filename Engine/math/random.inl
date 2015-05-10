
inline void Random::srand(int iSeed)
{
	mSeed = iSeed;
}

inline int Random::rand()
{
	return (((mSeed = mSeed * 214013L + 2531011L) >> 16) & kRandMax);
}

inline float Random::frand()
{
	return rand()%kRandMax / (float)kRandMax;
}



//////////////////////////////////////////////////////////////////////////



inline void Random::randQuadInternal()
{

	__declspec( align(16) ) __m128i cur_seed_split;

	__declspec( align(16) ) __m128i multiplier;

	__declspec( align(16) ) __m128i adder;

	__declspec( align(16) ) __m128i mod_mask;

	//__declspec( align(16) ) __m128i sra_mask;

//	__declspec( align(16) ) __m128i sseresult;

	__declspec( align(16) ) static const unsigned int mult[4] =

	{ 214013, 17405, 214013, 69069 };

	__declspec( align(16) ) static const unsigned int gadd[4] =

	{ 2531011, 10395331, 13737667, 1 };

	__declspec( align(16) ) static const unsigned int mask[4] =

	{ 0xFFFFFFFF, 0, 0xFFFFFFFF, 0 };

	__declspec( align(16) ) static const unsigned int masklo[4] =

	{ 0x00007FFF, 0x00007FFF, 0x00007FFF, 0x00007FFF };



	adder = _mm_load_si128( (__m128i*) gadd);

	multiplier = _mm_load_si128( (__m128i*) mult);

	mod_mask = _mm_load_si128( (__m128i*) mask);

	//sra_mask = _mm_load_si128( (__m128i*) masklo);

	cur_seed_split = _mm_shuffle_epi32( mQuadSeed, _MM_SHUFFLE( 2, 3, 0, 1 ) );



	mQuadSeed = _mm_mul_epu32( mQuadSeed, multiplier );

	multiplier = _mm_shuffle_epi32( multiplier, _MM_SHUFFLE( 2, 3, 0, 1 ) );

	cur_seed_split = _mm_mul_epu32( cur_seed_split, multiplier );


	mQuadSeed = _mm_and_si128( mQuadSeed, mod_mask);

	cur_seed_split = _mm_and_si128( cur_seed_split, mod_mask );

	cur_seed_split = _mm_shuffle_epi32( cur_seed_split, _MM_SHUFFLE( 2, 3, 0, 1 ) );

	mQuadSeed = _mm_or_si128( mQuadSeed, cur_seed_split );

	mQuadSeed = _mm_add_epi32( mQuadSeed, adder);



	//_mm_store_si128( (__m128i*) oAlignedIntQuad, mQuadSeed);

	return;

}


inline void Random::randQuad(unsigned* oAlignedIntQuad)
{
	randQuadInternal();
	_mm_store_si128( (__m128i*) oAlignedIntQuad, mQuadSeed);
}

inline void Random::frandQuad(float* oAlignedFloatQuad)
{
	randQuadInternal();

	// Add the lines below if you wish to reduce your results to 16-bit vals...
	__declspec( align(16) ) static const unsigned int masklo[4] =
	{ 0x00007FFF, 0x00007FFF, 0x00007FFF, 0x00007FFF };
	__declspec( align(16) ) static const float masklof[4] =
	{ 1.f/(float)0x00007FFF, 1.f/(float)0x00007FFF, 1.f/(float)0x00007FFF, 1.f/(float)0x00007FFF };

	__m128i sra_mask = _mm_load_si128( (__m128i*) masklo);
	__m128i sseresult = _mm_srai_epi32( mQuadSeed, 16);
	sseresult = _mm_and_si128( sseresult, sra_mask );
	
	__m128 multiplier = _mm_load_ps(masklof);
	__m128 floats = _mm_cvtepi32_ps(sseresult);
	floats = _mm_mul_ps(floats, multiplier);

	_mm_store_ps(oAlignedFloatQuad, floats);
}