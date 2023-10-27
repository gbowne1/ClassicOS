#pragma once

#include "EHS.h"

class HRNG
{
public:
	static UInt_64 GenerateSeed_u64();

	static UInt_64 Generate_u64(const UInt_64 min, const UInt_64 max);

	static UInt_64 Generate_u64();

	static SInt_64 GenerateSeed_s64();

	static SInt_64 Generate_s64(const SInt_64 min, const SInt_64 max);

	static SInt_64 Generate_s64();

	static UInt_32 GenerateSeed_u32();

	static UInt_32 Generate_u32(const UInt_32 min, const UInt_32 max);

	static UInt_32 Generate_u32();

	static SInt_32 GenerateSeed_s32();

	static SInt_32 Generate_s32(const SInt_32 min, const SInt_32 max);

	static SInt_32 Generate_s32();

	static UInt_32 GenerateSeed_u16();

	static UInt_16 Generate_u16(const UInt_16 min, const UInt_16 max);

	static UInt_16 Generate_u16();

	static SInt_16 GenerateSeed_s16();

	static SInt_16 Generate_s16(const SInt_16 min, const SInt_16 max);

	static SInt_16 Generate_s16();

	static UInt_8 GenerateSeed_u8();

	static UInt_8 Generate_u8(const UInt_8 min, const UInt_8 max);

	static UInt_8 Generate_u8();

	static SInt_8 GenerateSeed_s8();

	static SInt_8 Generate_s8(const SInt_8 min, const SInt_8 max);

	static SInt_8 Generate_s8();
};