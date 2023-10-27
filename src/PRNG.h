#pragma once

#include "EHS.h"

template<typename T>
class PRNG
{
private:
	T seed;
	T last;

public:
	PRNG()
		: seed(0), last(0)
	{
	}

	PRNG(const T seed)
		: seed(seed), last(seed)
	{
	}

	PRNG(const PRNG& prng)
		: seed(prng.seed), last(prng.last)
	{
	}

	PRNG& operator=(const PRNG& prng)
	{
		if (this == &prng)
			return *this;

		seed = prng.seec;
		last = prng.last;
	}

	T Generate(const T min, const T max)
	{
		return Generate() % (max - min) + min;
	}

	T Generate()
	{
		return ((last = last * 214013 + 2531011) >> 16) & 0x7fff;
	}
};

typedef PRNG<SInt_64> PRNG_s64;
typedef PRNG<UInt_64> PRNG_u64;
typedef PRNG<SInt_32> PRNG_s32;
typedef PRNG<UInt_32> PRNG_u32;
typedef PRNG<SInt_16> PRNG_s16;
typedef PRNG<UInt_16> PRNG_u16;
typedef PRNG<SInt_8> PRNG_s8;
typedef PRNG<UInt_8> PRNG_u8;