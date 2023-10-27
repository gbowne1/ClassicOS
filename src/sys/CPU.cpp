#include "CPU.h"

#include "../Log.h"
#include "../IO/File.h"
#include "../Json/Json.h"
#include "Thread.h"


{
    #ifdef OS_LINUX
		UInt_64 CPU::TSC_Freq = 0;
    #endif

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

	UInt_64 CPU::GetTSC_Freq()
	{
		#if defined(OS_WINDOWS)
			LARGE_INTEGER frequency = {};
			QueryPerformanceFrequency(&frequency);

			return frequency.QuadPart;
        #elif defined(OS_LINUX)
            if (!TSC_Freq)
                TSC_Freq = RetrieveTSC_Freq();

            return TSC_Freq;
		#endif

		return 0;
	}

	UInt_64 CPU::GetTSC()
	{
		#if defined(OS_WINDOWS)
			LARGE_INTEGER count = {};
            QueryPerformanceCounter(&count);

			return count.QuadPart;
        #elif defined(OS_LINUX)
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

	/*
	Str_8 CPU::ToStr()
	{
		return "Manufacturer: " + GetManufacturer() + "\r\n" +
				"Brand: " + GetBrand() + "\r\n" +
				"Stepping Id: " + Str_8::FromNum(GetSteppingId()) + "\r\n" +
				"GpuModel Id: " + Str_8::FromNum(GetModelId()) + "\r\n" +
				"Family Id: " + Str_8::FromNum(GetFamilyId()) + "\r\n" +
				"Processor Type Id: " + Str_8::FromNum(GetProcessorTypeId()) + "\r\n" +
				"Extended GpuModel Id: " + Str_8::FromNum(GetExtModelId()) + "\r\n" +
				"Extended Family Id: " + Str_8::FromNum(GetExtFamilyId()) + "\r\n" +
				"Has FPU: " + Str_8::FromNum((UInt_8)HasFPU()) + "\r\n" +
				"Has SSE: " + Str_8::FromNum((UInt_8)HasSSE()) + "\r\n" +
				"Has SSE 2: " + Str_8::FromNum((UInt_8)HasSSE2()) + "\r\n" +
				"Has SSE 3: " + Str_8::FromNum((UInt_8)HasSSE3()) + "\r\n" +
				"Has SSSE 3: " + Str_8::FromNum((UInt_8)HasSSSE3()) + "\r\n" +
				"Has SSE 4.1: " + Str_8::FromNum((UInt_8)HasSSE4_1()) + "\r\n" +
				"Has SSE 4.2: " + Str_8::FromNum((UInt_8)HasSSE4_2()) + "\r\n" +
				"Has AVX: " + Str_8::FromNum((UInt_8)HasAVX()) + "\r\n" +
				"Has RDRND: " + Str_8::FromNum((UInt_8)HasRDRND()) + "\r\n" +
				"Has AVX 2: " + Str_8::FromNum((UInt_8)HasAVX2()) + "\r\n" +
				"Has ADX: " + Str_8::FromNum((UInt_8)HasADX()) + "\r\n" +
				"Has RDSEED: " + Str_8::FromNum((UInt_8)HasRDSEED());
	}
	*/

	UInt_64 CPU::RetrieveTSC_Freq()
    {
        File tscDatabase("TSC_Frequencies.json", Mode::READ_WRITE, Disposition::CREATE_PERSISTENT);
        if (tscDatabase.Size())
        {
            Json json(tscDatabase.ReadStr_8(tscDatabase.Size()), 5);
            JsonObj* root = (JsonObj*)json.GetValue();

            Char_8 manu[13];
            manu[12] = 0;
            GetManufacturer(manu);

            JsonVar* jManu = root->GetVar(manu);
            if (jManu)
            {
                JsonObj* joManu = (JsonObj*)jManu->GetValue();

                Char_8 brand[49];
                brand[48] = 0;
                GetBrand(brand);

                JsonVar* jBrand = joManu->GetVar(brand);
                if (jBrand)
                {
                    tscDatabase.Release();

                    return (UInt_64)*(JsonNum*)jBrand->GetValue();
                }
                else
                {
					UInt_64 tscFreq = CalculateTSC_Freq();

                    joManu->AddVar({brand, tscFreq});

                    tscDatabase.WriteStr_8(json.ToStr(false));
                    tscDatabase.Release();

                    return tscFreq;
                }
            }
            else
            {
				UInt_64 tscFreq = CalculateTSC_Freq();

                Char_8 brand[49];
                brand[48] = 0;
                GetBrand(brand);

                JsonObj cpus(1, 0);
                cpus[0] = JsonVar(brand, tscFreq);

                JsonVar tmp({brand, cpus});
                root->AddVar(tmp);

                tscDatabase.WriteStr_8(json.ToStr(false));
                tscDatabase.Release();

                return tscFreq;
            }
        }

		UInt_64 tscFreq = CalculateTSC_Freq();

        Char_8 manu[13];
        manu[12] = 0;
        GetManufacturer(manu);

        Char_8 brand[49];
        brand[48] = 0;
        GetBrand(brand);

        JsonObj jManu(1, 0);
        jManu[0] = JsonVar(brand, tscFreq);

        JsonObj root(1, 0);
        root[0] = JsonVar(manu, jManu);

        Json json(root);

        tscDatabase.WriteStr_8(json.ToStr(false));
        tscDatabase.Release();

        return tscFreq;
    }

	UInt_64 CPU::CalculateTSC_Freq()
    {
		UInt_64 result = GetTSC();

        Thread::SleepFor(10000);

        return (GetTSC() - result) / 10;
    }
}