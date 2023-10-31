#include "CPU.h"

Architecture CPU::GetArchitecture()
{
	#if defined(ARCH_X64)
		return Architecture::X64;
	#elif defined(ARCH_ARM64)
		return Architecture::ARM64;
	#else
		return Architecture::UNKNOWN;
	#endif
}

UInt_8 CPU::PointerSize()
{
	return sizeof(void*);
}

Endianness CPU::GetEndianness()
{
	#if defined(LITTLE_ENDIAN)
		return Endianness::LE;
	#elif defined(BIG_ENDIAN)
		return Endianness::BE;
	#else
		UInt_16 tmp = 1;
		if (((Byte*)&tmp)[0] == 1)
			return Endianness::LE;

		return Endianness::BE;
	#endif
}

UInt_64 CPU::GetTSC()
{
	TSC tsc;
	RDTSCP(&tsc);

	#if defined(ARCH_X64)
		UInt_64 result = 0;

		#if defined(LITTLE_ENDIAN)
			((UInt_32*)&result)[0] = tsc.lowCount;
			((UInt_32*)&result)[1] = tsc.highCount;
		#elif defined(BIG_ENDIAN)
			((UInt_32*)&result)[0] = tsc.highCount;
			((UInt_32*)&result)[1] = tsc.lowCount;
		#endif

		return result;
	#elif defined(ARCH_X86)
		return tsc.lowPart;
	#endif

	return 0;
}

UInt_8 CPU::GetSteppingId()
{
	return (UInt_8)GetInfoBits() & 0x00001111;
}

UInt_8 CPU::GetModelId()
{
	return (UInt_8)GetInfoBits() & 0x11110000;
}

UInt_8 CPU::GetFamilyId()
{
	return (UInt_8)(GetInfoBits() >> 8) & 0x00001111;
}

UInt_8 CPU::GetProcessorTypeId()
{
	return (UInt_8)(GetInfoBits() >> 12) & 0x00000011;
}

UInt_8 CPU::GetExtModelId()
{
	return (UInt_8)(GetInfoBits() >> 16) & 0x00001111;
}

UInt_8 CPU::GetExtFamilyId()
{
	return (UInt_8)(GetInfoBits() >> 20);
}

bool CPU::HasFPU()
{
	return GetFeatureBits_1() & 0b00000000000000000000000000000001;
}

bool CPU::HasVME()
{
	return GetFeatureBits_1() & 0b00000000000000000000000000000010;
}

bool CPU::HasDE()
{
	return GetFeatureBits_1() & 0b00000000000000000000000000000100;
}

bool CPU::HasPSE()
{
	return GetFeatureBits_1() & 0b00000000000000000000000000001000;
}

bool CPU::HasTSC()
{
	return GetFeatureBits_1() & 0b00000000000000000000000000010000;
}

bool CPU::HasMSR()
{
	return GetFeatureBits_1() & 0b00000000000000000000000000100000;
}

bool CPU::HasPAE()
{
	return GetFeatureBits_1() & 0b00000000000000000000000001000000;
}

bool CPU::HasMCE()
{
	return GetFeatureBits_1() & 0b00000000000000000000000010000000;
}

bool CPU::HasCX8()
{
	return GetFeatureBits_1() & 0b00000000000000000000000100000000;
}

bool CPU::HasAPIC()
{
	return GetFeatureBits_1() & 0b00000000000000000000001000000000;
}

bool CPU::HasSEP()
{
	return GetFeatureBits_1() & 0b00000000000000000000100000000000;
}

bool CPU::HasMTRR()
{
	return GetFeatureBits_1() & 0b00000000000000000001000000000000;
}

bool CPU::HasPGE()
{
	return GetFeatureBits_1() & 0b00000000000000000010000000000000;
}

bool CPU::HasMCA()
{
	return GetFeatureBits_1() & 0b00000000000000000100000000000000;
}

bool CPU::HasCMOV()
{
	return GetFeatureBits_1() & 0b00000000000000001000000000000000;
}

bool CPU::HasPAT()
{
	return GetFeatureBits_1() & 0b00000000000000010000000000000000;
}

bool CPU::HasPSE_36()
{
	return GetFeatureBits_1() & 0b00000000000000100000000000000000;
}

bool CPU::HasPSN()
{
	return GetFeatureBits_1() & 0b00000000000001000000000000000000;
}

bool CPU::HasCLFSH()
{
	return GetFeatureBits_1() & 0b00000000000010000000000000000000;
}

bool CPU::HasDS()
{
	return GetFeatureBits_1() & 0b00000000001000000000000000000000;
}

bool CPU::HasACPI()
{
	return GetFeatureBits_1() & 0b00000000010000000000000000000000;
}

bool CPU::HasMMX()
{
	return GetFeatureBits_1() & 0b00000000100000000000000000000000;
}

bool CPU::HasFXSR()
{
	return GetFeatureBits_1() & 0b00000001000000000000000000000000;
}

bool CPU::HasSSE()
{
	return GetFeatureBits_1() & 0b00000010000000000000000000000000;
}

bool CPU::HasSSE2()
{
	return GetFeatureBits_1() & 0b00000100000000000000000000000000;
}

bool CPU::HasSS()
{
	return GetFeatureBits_1() & 0b00001000000000000000000000000000;
}

bool CPU::HasHTT()
{
	return GetFeatureBits_1() & 0b00010000000000000000000000000000;
}

bool CPU::HasTM()
{
	return GetFeatureBits_1() & 0b00100000000000000000000000000000;
}

bool CPU::HasIA64()
{
	return GetFeatureBits_1() & 0b01000000000000000000000000000000;
}

bool CPU::HasPBE()
{
	return GetFeatureBits_1() & 0b10000000000000000000000000000000;
}

bool CPU::HasSSE3()
{
	return GetFeatureBits_2() & 0b00000000000000000000000000000001;
}

bool CPU::HasPCLMULQDQ()
{
	return GetFeatureBits_2() & 0b00000000000000000000000000000010;
}

bool CPU::HasDTES64()
{
	return GetFeatureBits_2() & 0b00000000000000000000000000000100;
}

bool CPU::HasMONITOR()
{
	return GetFeatureBits_2() & 0b00000000000000000000000000001000;
}

bool CPU::HasDS_CPL()
{
	return GetFeatureBits_2() & 0b00000000000000000000000000010000;
}

bool CPU::HasVMX()
{
	return GetFeatureBits_2() & 0b00000000000000000000000000100000;
}

bool CPU::HasSMX()
{
	return GetFeatureBits_2() & 0b00000000000000000000000001000000;
}

bool CPU::HasEST()
{
	return GetFeatureBits_2() & 0b00000000000000000000000010000000;
}

bool CPU::HasTM2()
{
	return GetFeatureBits_2() & 0b00000000000000000000000100000000;
}

bool CPU::HasSSSE3()
{
	return GetFeatureBits_2() & 0b00000000000000000000001000000000;
}

bool CPU::HasCNXT_ID()
{
	return GetFeatureBits_2() & 0b00000000000000000000010000000000;
}

bool CPU::HasSDBG()
{
	return GetFeatureBits_2() & 0b00000000000000000000100000000000;
}

bool CPU::HasFMA()
{
	return GetFeatureBits_2() & 0b00000000000000000001000000000000;
}

bool CPU::HasCX16()
{
	return GetFeatureBits_2() & 0b00000000000000000010000000000000;
}

bool CPU::HasXTPR()
{
	return GetFeatureBits_2() & 0b00000000000000000100000000000000;
}

bool CPU::HasPDCM()
{
	return GetFeatureBits_2() & 0b00000000000000001000000000000000;
}

bool CPU::HasPCID()
{
	return GetFeatureBits_2() & 0b00000000000000100000000000000000;
}

bool CPU::HasDCA()
{
	return GetFeatureBits_2() & 0b00000000000001000000000000000000;
}

bool CPU::HasSSE4_1()
{
	return GetFeatureBits_2() & 0b00000000000010000000000000000000;
}

bool CPU::HasSSE4_2()
{
	return GetFeatureBits_2() & 0b00000000000100000000000000000000;
}

bool CPU::HasX2APIC()
{
	return GetFeatureBits_2() & 0b00000000001000000000000000000000;
}

bool CPU::HasMOVBE()
{
	return GetFeatureBits_2() & 0b00000000010000000000000000000000;
}

bool CPU::HasPOPCNT()
{
	return GetFeatureBits_2() & 0b00000000100000000000000000000000;
}

bool CPU::HasTSC_DEADLINE()
{
	return GetFeatureBits_2() & 0b00000001000000000000000000000000;
}

bool CPU::HasAES()
{
	return GetFeatureBits_2() & 0b00000010000000000000000000000000;
}

bool CPU::HasXSAVE()
{
	return GetFeatureBits_2() & 0b00000100000000000000000000000000;
}

bool CPU::HasOSXSAVE()
{
	return GetFeatureBits_2() & 0b00001000000000000000000000000000;
}

bool CPU::HasAVX()
{
	return GetFeatureBits_2() & 0b00010000000000000000000000000000;
}

bool CPU::HasF16C()
{
	return GetFeatureBits_2() & 0b00100000000000000000000000000000;
}

bool CPU::HasRDRND()
{
	return GetFeatureBits_2() & 0b01000000000000000000000000000000;
}

bool CPU::HasHYPERVISOR()
{
	return GetFeatureBits_2() & 0b10000000000000000000000000000000;
}

bool CPU::HasAVX2()
{
	return GetExtFeatureBits_1() & 0b00000000000000000000000000100000;
}

bool CPU::HasRDSEED()
{
	return GetExtFeatureBits_1() & 0b00000000000001000000000000000000;
}

bool CPU::HasADX()
{
	return GetExtFeatureBits_1() & 0b00000000000010000000000000000000;
}