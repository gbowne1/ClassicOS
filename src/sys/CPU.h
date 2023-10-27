#pragma once

#include "../EHS.h"
#include "../Str.h"
#include "../Array.h"

enum class Architecture : UInt_8
{
	X64,
	X86,
	ARM64,
	ARM,
	UNKNOWN
};

enum class Endianness : UInt_8
{
	LE,
	BE
};

struct TSC
{
	UInt_32 coreId = 0;
	UInt_32 highCount = 0;
	UInt_32 lowCount = 0;
};

class CPU
{
public:
	static Architecture GetArchitecture();

	static UInt_8 PointerSize();

	static Endianness GetEndianness();

	static void RDTSCP(TSC* tsc);

	static UInt_64 GetTSC();

	/// Retrieves the CPU manufacturer id as a null-terminated ASCII string.
	/// @param[out] input A twelve byte character array representing the manufacturer id.
	static void GetManufacturer(Char_8* input);

	static UInt_32 GetInfoBits();

	static UInt_8 GetSteppingId();

	static UInt_8 GetModelId();

	static UInt_8 GetFamilyId();

	static UInt_8 GetProcessorTypeId();

	static UInt_8 GetExtModelId();

	static UInt_8 GetExtFamilyId();

	static UInt_32 GetFeatureBits_1();

	static bool HasFPU();

	static bool HasVME();

	static bool HasDE();

	static bool HasPSE();

	static bool HasTSC();

	static bool HasMSR();

	static bool HasPAE();

	static bool HasMCE();

	static bool HasCX8();

	static bool HasAPIC();

	static bool HasSEP();

	static bool HasMTRR();

	static bool HasPGE();

	static bool HasMCA();

	static bool HasCMOV();

	static bool HasPAT();

	static bool HasPSE_36();

	static bool HasPSN();

	static bool HasCLFSH();

	static bool HasDS();

	static bool HasACPI();

	static bool HasMMX();

	static bool HasFXSR();

	static bool HasSSE();

	static bool HasSSE2();

	static bool HasSS();

	static bool HasHTT();

	static bool HasTM();

	static bool HasIA64();

	static bool HasPBE();

	static UInt_32 GetFeatureBits_2();

	static bool HasSSE3();

	static bool HasPCLMULQDQ();

	static bool HasDTES64();

	static bool HasMONITOR();

	static bool HasDS_CPL();

	static bool HasVMX();

	static bool HasSMX();

	static bool HasEST();

	static bool HasTM2();

	static bool HasSSSE3();

	static bool HasCNXT_ID();

	static bool HasSDBG();

	static bool HasFMA();

	static bool HasCX16();

	static bool HasXTPR();

	static bool HasPDCM();

	static bool HasPCID();

	static bool HasDCA();

	static bool HasSSE4_1();

	static bool HasSSE4_2();

	static bool HasX2APIC();

	static bool HasMOVBE();

	static bool HasPOPCNT();

	static bool HasTSC_DEADLINE();

	static bool HasAES();

	static bool HasXSAVE();

	static bool HasOSXSAVE();

	static bool HasAVX();

	static bool HasF16C();

	static bool HasRDRND();

	static bool HasHYPERVISOR();

	static UInt_32 GetExtFeatureBits_1();

	static bool HasAVX2();

	static bool HasRDSEED();

	static bool HasADX();

	static UInt_32 GetExtFeatureBits_2();

	static UInt_32 GetExtFeatureBits_3();

	/// Retrieves the CPU brand as a null-terminated ASCII string.
	/// @param[out] input A 48 byte character array representing the brand.
	static void GetBrand(Char_8* input);

	//static Str_8 ToStr();
};