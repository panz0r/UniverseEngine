#pragma once

namespace ue
{


unsigned murmur3_32(const char *key, unsigned len, unsigned seed);

inline unsigned default_hash(const void *key, unsigned len, unsigned seed)
{
	return murmur3_32((const char*)key, len, seed);
}

}