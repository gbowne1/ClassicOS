#include "Util.h"

bool Util::IsEqual(const void* const a, const void* const b, const UInt_64 size)
{
	Byte* aBytes = (Byte*)a;
	Byte* bBytes = (Byte*)b;
	UInt_64 remainder = size;
	UInt_64 i = 0;

	while (i < size)
	{
		if (remainder >= sizeof(UInt_64))
		{
			if (*(UInt_64*)&aBytes[i] != *(UInt_64*)&bBytes[i])
				return false;

			i += sizeof(UInt_64);
		}
		else if (remainder >= sizeof(UInt_32))
		{
			if (*(UInt_32*)&aBytes[i] != *(UInt_32*)&bBytes[i])
				return false;

			i += sizeof(UInt_32);
		}
		else if (remainder >= sizeof(UInt_16))
		{
			if (*(UInt_16*)&aBytes[i] != *(UInt_16*)&bBytes[i])
				return false;

			i += sizeof(UInt_16);
		}
		else
		{
			if (aBytes[i] != bBytes[i])
				return false;

			i += sizeof(Byte);
		}

		remainder = size - i;
	}

	return true;
}

void Util::Copy(void* const out, const void* const in, const UInt_64 size)
{
	Byte* outB = (Byte*)out;
	Byte* inB = (Byte*)in;
	UInt_64 remainder = size;

	UInt_64 i = 0;
	while (i < size)
	{
		if (remainder >= sizeof(UInt_64))
		{
			*(UInt_64*)&outB[i] = *(UInt_64*)&inB[i];
			i += sizeof(UInt_64);
		}
		else if (remainder >= sizeof(UInt_32))
		{
			*(UInt_32*)&outB[i] = *(UInt_32*)&inB[i];
			i += sizeof(UInt_32);
		}
		else if (remainder >= sizeof(UInt_16))
		{
			*(UInt_16*)&outB[i] = *(UInt_16*)&inB[i];
			i += sizeof(UInt_16);
		}
		else
		{
			outB[i++] = inB[i];
		}

		remainder = size - i;
	}
}

void Util::Zero(void* const in, const UInt_64 size)
{
	Byte* inB = (Byte*)in;
	UInt_64 remainder = size;
	UInt_64 i = 0;
	while (i < size)
	{
		if (remainder >= sizeof(UInt_64))
		{
			*(UInt_64*)&inB[i] = 0;
			i += sizeof(UInt_64);
		}
		else if (remainder >= sizeof(UInt_32))
		{
			*(UInt_32*)&inB[i] = 0;
			i += sizeof(UInt_32);
		}
		else if (remainder >= sizeof(UInt_16))
		{
			*(UInt_16*)&inB[i] = 0;
			i += sizeof(UInt_16);
		}
		else
		{
			inB[i++] = 0;
		}

		remainder = size - i;
	}
}