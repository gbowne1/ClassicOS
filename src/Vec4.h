#pragma once

#include "EHS.h"
#include "UTF.h"
#include "Str.h"
#include "Math.h"
#include "Vec2.h"
#include "Vec3.h"

template<typename T>
class Vec4
{
public:
	T x;
	T y;
	T z;
	T w;

	Vec4(const T scalar = 0)
			: x(scalar), y(scalar), z(scalar), w(scalar)
	{
	}

	Vec4(const T x, const T y, const T z, const T w)
		: x(x), y(y), z(z), w(w)
	{
	}

	template<typename C>
	Vec4(const Vec2<C>& vec, const T z = 0, const T w = 0)
			: x((T)vec.x), y((T)vec.y), z(z), w(w)
	{
	}

	template<typename C>
	Vec4(const Vec3<C>& vec, const T w = 1)
			: x((T)vec.x), y((T)vec.y), z((T)vec.z), w(w)
	{
	}

	template<typename C>
	Vec4(const Vec4<C>& vec)
		: x((T)vec.x), y((T)vec.y), z((T)vec.z), w((T)vec.w)
	{
	}

	template<typename C>
	Vec4<T>& operator=(const Vec2<C>& vec)
	{
		x = (T)vec.x;
		y = (T)vec.y;
		z = (T)0;
		w = (T)0;

		return*this;
	}

	template<typename C>
	Vec4<T>& operator=(const Vec3<C>& vec)
	{
		x = (T)vec.x;
		y = (T)vec.y;
		z = (T)vec.z;
		w = (T)0;

		return*this;
	}

	template<typename C>
	Vec4<T>& operator=(const Vec4<C>& vec)
	{
		x = (T)vec.x;
		y = (T)vec.y;
		z = (T)vec.z;
		w = (T)vec.w;

		return*this;
	}

	bool operator==(const Vec4<T>& vec)
	{
		return Math::ComCmp(x, vec.x) && Math::ComCmp(y, vec.y) && Math::ComCmp(z, vec.z) && Math::ComCmp(w, vec.w);
	}

	bool operator!=(const Vec4<T>& vec)
	{
		return !Math::ComCmp(z, vec.z) || !Math::ComCmp(y, vec.y) || !Math::ComCmp(z, vec.z) || !Math::ComCmp(w, vec.w);
	}

	Vec4<T>& operator+=(const Vec4<T>& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		w += vec.w;

		return *this;
	}

	Vec4<T> operator+(const Vec4<T>& vec)
	{
		Vec4<T> tmp;

		tmp.x = x + vec.x;
		tmp.y = y + vec.y;
		tmp.z = z + vec.z;
		tmp.w = w + vec.w;

		return tmp;
	}

	Vec4<T>& operator+=(const T scalar)
	{
		x += scalar;
		y += scalar;
		z += scalar;
		w += scalar;

		return *this;
	}

	Vec4<T> operator+(const T scalar)
	{
		Vec4<T> tmp;

		tmp.x = x + scalar;
		tmp.y = y + scalar;
		tmp.z = z + scalar;
		tmp.w = w + scalar;

		return tmp;
	}

	Vec4<T>& operator-=(const Vec4<T>& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		w -= vec.w;

		return *this;
	}

	Vec4<T> operator-(const Vec4<T>& vec)
	{
		Vec4<T> tmp;

		tmp.x = x - vec.x;
		tmp.y = y - vec.y;
		tmp.z = z - vec.z;
		tmp.w = w - vec.w;

		return tmp;
	}

	Vec4<T>& operator-=(const T scalar)
	{
		x -= scalar;
		y -= scalar;
		z -= scalar;
		w -= scalar;

		return *this;
	}

	Vec4<T> operator-(const T scalar)
	{
		Vec4<T> tmp;

		tmp.x = x - scalar;
		tmp.y = y - scalar;
		tmp.z = z - scalar;
		tmp.w = w - scalar;

		return tmp;
	}

	Vec4<T>& operator*=(const Vec4<T>& vec)
	{
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
		w *= vec.w;

		return *this;
	}

	Vec4<T> operator*(const Vec4<T>& vec)
	{
		Vec4<T> tmp;

		tmp.x = x * vec.x;
		tmp.y = y * vec.y;
		tmp.z = z * vec.z;
		tmp.w = w * vec.w;

		return tmp;
	}

	Vec4<T>& operator*=(const T scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;

		return *this;
	}

	Vec4<T> operator*(const T scalar)
	{
		Vec4<T> tmp;

		tmp.x = x * scalar;
		tmp.y = y * scalar;
		tmp.z = z * scalar;
		tmp.w = w * scalar;

		return tmp;
	}

	Vec4<T>& operator/=(const Vec4<T>& vec)
	{
		x /= vec.x;
		y /= vec.y;
		z /= vec.z;
		w /= vec.w;

		return *this;
	}

	Vec4<T> operator/(const Vec4<T>& vec)
	{
		Vec4<T> tmp;

		tmp.x = x / vec.x;
		tmp.y = y / vec.y;
		tmp.z = z / vec.z;
		tmp.w = w / vec.w;

		return tmp;
	}

	Vec4<T>& operator/=(const T scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;

		return *this;
	}

	Vec4<T> operator/(const T scalar)
	{
		Vec4<T> tmp;

		tmp.x = x / scalar;
		tmp.y = y / scalar;
		tmp.z = z / scalar;
		tmp.w = w / scalar;

		return tmp;
	}

	T operator[](const UInt_64 index) const
	{
		switch (index)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			return x;
		}
	}

	T& operator[](const UInt_64 index)
	{
		switch (index)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			return x;
		}
	}

	operator Vec3<T>()
	{
		return Vec3<T>(x, y, z);
	}

	operator Vec2<T>()
	{
		return Vec2<T>(x, y);
	}

	T GetDotProduct(const Vec4<T>& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
	}

	T GetMagnitude() const
	{
		return Math::Sqrt(x * x + y * y + z * z + w * w);
	}

	T GetMagnitude2() const
	{
		return x * x + y * y + z * z + w * w;
	}
};

typedef Vec4<UInt_64> Vec4_u64;
typedef Vec4<SInt_64> Vec4_s64;
typedef Vec4<Int_64> Vec4_64;
typedef Vec4<UInt_32> Vec4_u32;
typedef Vec4<SInt_32> Vec4_s32;
typedef Vec4<Int_32> Vec4_32;
typedef Vec4<UInt_16> Vec4_u16;
typedef Vec4<SInt_16> Vec4_s16;
typedef Vec4<Int_16> Vec4_16;
typedef Vec4<UInt_8> Vec4_u8;
typedef Vec4<SInt_8> Vec4_s8;
typedef Vec4<Int_8> Vec4_8;
typedef Vec4<float> Vec4_f;
typedef Vec4<double> Vec4_d;