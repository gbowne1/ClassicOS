#include "Math.h"

bool Math::AbsCmp(const float a, const float b)
{
	return Abs(a - b) <= fltEpsilon;
}

bool Math::AbsCmp(const double a, const double b)
{
	return Abs(a - b) <= fltEpsilon;
}

bool Math::RelCmp(const float a, const float b)
{
	return Abs(a - b) <= fltEpsilon * Max(Abs(a), Abs(b));
}

bool Math::RelCmp(const double a, const double b)
{
	return Abs(a - b) <= fltEpsilon * Max(Abs(a), Abs(b));
}

bool Math::ComCmp(const float a, const float b)
{
	return Abs(a - b) <= fltEpsilon * Max(1.0f, Max(Abs(a), Abs(b)));
}

bool Math::ComCmp(const double a, const double b)
{
	return Abs(a - b) <= dblEpsilon * Max(1.0, Max(Abs(a), Abs(b)));
}

double Math::Sqrt(const double from)
{
	#if defined(ARCH_X64)
		if (CPU::HasAVX())
			return Sqrt_AVX(from);
		else if (CPU::HasSSE())
			return Sqrt_SSE2(from);

		double temp = 0.0;
		double result = from / 2.0;

		while (result != temp)
		{
			temp = result;
			result = (from / temp + temp) / 2.0;
		}

		return result;
	#elif defined(ARCH_ARM64)
		return Sqrt_VFP4(from);
	#endif
}

float Math::Sqrt(const float from)
{
	#if defined(ARCH_X64) || defined(ARCH_X86)
		if (CPU::HasAVX())
			return Sqrt_AVX(from);
		else if (CPU::HasSSE())
			return Sqrt_SSE(from);

		float temp = 0.0f;
		float result = from / 2.0f;

		while (result != temp)
		{
			temp = result;
			result = (from / temp + temp) / 2.0f;
		}

		return result;
	#elif defined(ARCH_ARM64)
		return Sqrt_VFP4(from);
	#endif
}

float Math::Mod(const float from, const float divisor)
{
	return from - Trunc(from / divisor) * divisor;
}

double Math::Mod(const double from, const double divisor)
{
	return from - Trunc(from / divisor) * divisor;
}