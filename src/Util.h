#pragma once

#include "EHS.h"

class Util
{
public:
	static bool IsEqual(const void* const a, const void* const b, const UInt_64 size);

	static void Copy(void* const out, const void* const in, const UInt_64 size);

	static void Zero(void* const in, const UInt_64 size);
};