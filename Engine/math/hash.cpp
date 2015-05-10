#include "hash.h"

namespace ue
{

unsigned murmur3_32(const char *key, unsigned len, unsigned seed)
{
	static const unsigned c1 = 0xcc9e2d51;
	static const unsigned c2 = 0x1b873593;
	static const unsigned r1 = 15;
	static const unsigned r2 = 13;
	static const unsigned m = 5;
	static const unsigned n = 0xe6546b64;

	unsigned hash = seed;

	const int nblocks = len / 4;
	const unsigned *blocks = (const unsigned *)key;
	int i;
	for (i = 0; i < nblocks; i++) {
		unsigned k = blocks[i];
		k *= c1;
		k = (k << r1) | (k >> (32 - r1));
		k *= c2;

		hash ^= k;
		hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
	}

	const char *tail = (const char *)(key + nblocks * 4);
	unsigned k1 = 0;

	switch (len & 3) {
	case 3:
		k1 ^= tail[2] << 16;
	case 2:
		k1 ^= tail[1] << 8;
	case 1:
		k1 ^= tail[0];

		k1 *= c1;
		k1 = (k1 << r1) | (k1 >> (32 - r1));
		k1 *= c2;
		hash ^= k1;
	}

	hash ^= len;
	hash ^= (hash >> 16);
	hash *= 0x85ebca6b;
	hash ^= (hash >> 13);
	hash *= 0xc2b2ae35;
	hash ^= (hash >> 16);

	return hash;
}

}

