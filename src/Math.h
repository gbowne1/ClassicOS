#pragma once

#include "sys/cpu.h"

#define LOW_WORD(x) *((int*)&x) + 1

class Math
{
private:
	static float Sqrt_AVX(const float from);

	static double Sqrt_AVX(const double from);

	static float Sqrt_SSE(const float from);

	static double Sqrt_SSE2(const double from);

	static float Sqrt_VFP4(const float from);

	static double Sqrt_VFP4(const double from);

public:
	constexpr static float fltEpsilon = 1e-7f;
	constexpr static double dblEpsilon = 1e-16;

	/// Absolute tolerance comparison for single precision floats.
	static bool AbsCmp(const float a, const float b);

	/// Absolute tolerance comparison for double precision floats.
	static bool AbsCmp(const double a, const double b);

	/// Relative tolerance comparison for single precision floats.
	static bool RelCmp(const float a, const float b);

	/// Relative tolerance comparison for double precision floats.
	static bool RelCmp(const double a, const double b);

	/// Combined absolute and relative tolerance comparison for single precision floats.
	static bool ComCmp(const float a, const float b);

	/// Combined absolute and relative tolerance comparison for double precision floats.
	static bool ComCmp(const double a, const double b);

	template<typename T = float>
	static T Max(const T a, const T b)
	{
		return a > b ? a : b;
	}

	template<typename T = float>
	static T Min(const T a, const T b)
	{
		return a < b ? a : b;
	}

	template<typename T = float>
	static T Clamp(const T value, const T min, const T max)
	{
		if (value < min)
			return min;
		else if (value > max)
			return max;

		return value;
	}

	template<typename T = float>
	static T Abs(const T from)
	{
		return from < 0 ? -from : from;
	}

	/// Retrieves a very accurate version of Pi as a long double and converts it.
	/// @tparam T The data type to return Pi as.
	/// @returns The result.
	template<typename T = float>
	static constexpr T Pi()
	{
		return (T)3.141592653589793238462643383279502884L;
	}

	/// Converts degrees into radians.
	/// @tparam T The data type to return;
	/// @param [in] from The value to convert to radians.
	/// @returns The value in radians.
	template<typename T = float>
	static T Rads(const T from)
	{
		return from * 0.01745329251994329576923690768489;
	}

	/// Converts radians into degrees.
	/// @tparam T The data type to return;
	/// @param [in] from The value to convert to degrees.
	/// @returns The value in degrees.
	template<typename T = float>
	static T Degr(const T from)
	{
		return from * 57.295779513082320876798154814105;
	}

	/// A method for use of exponents.
	/// @tparam T The data type to return;
	/// @tparam I The data type to use as the exponent.
	/// @param [in] from The value to use the exponent on.
	/// @param [in] of The exponent.
	/// @returns The result.
	template<typename T = float, typename I = UInt_64>
	static T Pow(const T from, const I of)
	{
		if (of < 0)
		{
			if (from == 0)
				return -0;

			return 1 / (from * Pow<T>(from, (-of) - 1));
		}

		if (of == 0)
			return 1;
		else if (of == 1)
			return from;

		return from * Pow<T>(from, of - 1);
	}

	static float Near(const float from);

	static double Near(const double from);

	static float Floor(const float from);

	static double Floor(const double from);

	static float Ceil(const float from);

	static double Ceil(const double from);

	static float Trunc(const float from);

	static double Trunc(const double from);

	static float Mod(const float from, const float divisor);

	static double Mod(const double from, const double divisor);

	/// A method for retrieving the square root of a value.
	/// @tparam T The data type to use.
	/// @param [in] from The value to retrieve to square root of.
	/// @returns The result.
	template<typename T = float>
	static T Sqrt(const T from)
	{
		T temp = 0;
		T result = from / 2;

		while (result != temp)
		{
			temp = result;
			result = (from / temp + temp) / 2;
		}

		return result;
	}

	static double Sqrt(const double from);

	static float Sqrt(const float from);

	template<typename R = float>
	static R Sin(const R angle, const R precision = 0.001)
	{
		R sum = angle;
		R term = angle;

		for (UInt_64 i = 1; Abs<R>(term) >= precision; ++i)
		{
			term *= -angle * angle / (R)((2 * i + 1) * (2 * i));
			sum += term;
		}

		return sum;

		/*
		R sum = 0;

		for (USize n = 0; n < precision; ++n)
			sum += Pow<R>(-1, n) / (R)Fact<T>(2 * n + 1) * Pow<R>(angle, 2 * n + 1);

		return sum;
		*/
	}

	template<typename R = float, typename T = UInt_64>
	static R ASin(const R yPos, const T precision = 10)
	{
		R sum = 0;

		for (T n = 0; n < precision; ++n)
			sum += (R)Fact<T>(2 * n) / (Pow<R>(4, n) * Pow<R>((R)Fact<T>(n), 2) * (2 * n + 1)) * Pow<R>(yPos, 2 * n + 1);

		return sum;
	}

	/// A trigonometry Cosine function for finding the X-Axis position from the Z-Axis angle.
	/// @tparam R Input and result data type.
	/// @tparam T Precision data type.
	/// @param[in] angle The angle in radians from the Z-Axis.
	/// @param [in] precision Sigma max.
	/// @returns The X-Axis position.
	template<typename R = float>
	static R Cos(const R angle, const R precision = 0.001)
	{
		R sum = 1.0;
		R term = 1.0;

		for (UInt_64 i = 2; Abs<R>(term) >= precision; i += 2)
		{
			term *= -angle * angle / (R)(i * (i - 1));
			sum += term;
		}

		return sum;

		/*
		R sum = 0;

		for (T n = 0; n < precision; ++n)
			sum += Pow<R>(-1, n) / (R)Fact<T>(2 * n) * Pow<R>(angle, 2 * n);

		return sum;
		*/
	}

	/// A trigonometry Arc Cosine function for finding the Z-Axis angle form the X-Axis position.
	/// @tparam R Input and result data type.
	/// @tparam T Precision data type.
	/// @param [in] xPos The position from the X-Axis.
	/// @param [in] precision Sigma max.
	/// @returns The Z-Axis angle.
	template<typename R = float, typename T = UInt_64>
	static R ACos(const R xPos, const T precision = 10)
	{
		return Pi<R>() / 2 - ASin<R, T>(xPos, precision);
	}

	template<typename R = float>
	static R Tan(const R angle, const R precision = 0.001)
	{
		/*
		R sum = 0;

		for (T n = 1; n < precision + 1; ++n)
			sum += B<R>(2 * n) * Pow<R>(-4, n) * (1 - Pow<R>(4, n)) / (R)Fact<T>(2 * n) * Pow<R>(angle, 2 * n - 1);

		return sum;
		*/
		return Sin<R>(angle) / Cos<R>(angle);
	}

	template<typename R = float, typename T = UInt_64>
	static R ATan(const R x, const T precision = 1)
	{
		R sum = 0;

		for (T n = 0; n < precision; ++n)
			sum += Pow<R>(-1, n) / (2 * n + 1) * Pow<R>(x, 2 * n + 1);

		return sum;
	}

	template<typename R = float>
	static R Cot(const R x, const R precision = 0.001)
	{
		return 1 / Tan<R>(x, precision);
	}

	template<typename T = UInt_64>
	static T Fact(const T n)
	{
		if (n <= 1)
			return 1;

		return n * Fact<T>(n - 1);
	}

	template<typename R = float, typename T = UInt_64>
	static R Combination(const T n, const T k)
	{
		if (k <= n)
			return (R)Fact<T>(n) / (R)(Fact<T>(n - k) * Fact<T>(k));

		return 0;
	}

	template<typename R = float, typename T = UInt_64>
	static R B(const T n)
	{
		R innerSum = 0;
		R outerSum = 0;

		for (T k = 0; k <= n; ++k)
		{
			for (T r = 0; r <= k; ++r)
				innerSum += Pow<R, T>(-1, r) * Combination<R, T>(k, r) * Pow<R, T>(r, n);

			outerSum += 1 / ((R)k + 1) * innerSum;
			innerSum = 0;
		}

		return outerSum;
	}
};