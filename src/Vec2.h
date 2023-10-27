#pragma once

#include "EHS.h"
#include "UTF.h"
#include "Str.h"
#include "Math.h"

template<typename T = float>
class Vec2
{
public:
	T x;
	T y;

	Vec2(const T x, const T y)
		: x(x), y(y)
	{
	}

	template<typename C>
	Vec2(const Vec2<C>& vec)
		: x((T)vec.x), y((T)vec.y)
	{
	}

	Vec2(const T scalar = 0)
		: x(scalar), y(scalar)
	{
	}

	template<typename C>
	Vec2<T>& operator=(const Vec2<C>& vec)
	{
		x = (T)vec.x;
		y = (T)vec.y;

		return *this;
	}

	bool operator==(const Vec2<T>& vec) const
	{
		return Math::ComCmp(x, vec.x) && Math::ComCmp(x, vec.y);
	}

	bool operator!=(const Vec2<T>& vec) const
	{
		return !Math::ComCmp(x, vec.x) || !Math::ComCmp(x, vec.y);
	}

	Vec2<T>& operator+=(const Vec2<T>& vec)
	{
		x += vec.x;
		y += vec.y;

		return *this;
	}

	Vec2<T> operator+(const Vec2<T>& vec) const
	{
		Vec2<T> tmp;

		tmp.x = x + vec.x;
		tmp.y = y + vec.y;

		return tmp;
	}

	Vec2<T>& operator+=(const T scalar)
	{
		x += scalar;
		y += scalar;

		return *this;
	}

	Vec2<T> operator+(const T scalar) const
	{
		Vec2<T> tmp;

		tmp.x = x + scalar;
		tmp.y = y + scalar;

		return tmp;
	}

	Vec2<T>& operator-=(const Vec2<T>& vec)
	{
		x -= vec.x;
		y -= vec.y;

		return *this;
	}

	Vec2<T> operator-(const Vec2<T>& vec) const
	{
		Vec2<T> tmp;

		tmp.x = x - vec.x;
		tmp.y = y - vec.y;

		return tmp;
	}

	Vec2<T>& operator-=(const T scalar)
	{
		x -= scalar;
		y -= scalar;

		return *this;
	}

	Vec2<T> operator-(const T scalar) const
	{
		Vec2<T> tmp;

		tmp.x = x - scalar;
		tmp.y = y - scalar;

		return tmp;
	}

	Vec2<T>& operator/=(const Vec2<T>& vec)
	{
		x /= vec.x;
		y /= vec.y;

		return *this;
	}

	Vec2<T> operator/(const Vec2<T>& vec) const
	{
		Vec2<T> tmp;

		tmp.x = x / vec.x;
		tmp.y = y / vec.y;

		return tmp;
	}

	Vec2<T>& operator/=(const T scalar)
	{
		x /= scalar;
		y /= scalar;

		return *this;
	}

	Vec2<T> operator/(const T scalar) const
	{

		Vec2<T> tmp;

		tmp.x = x / scalar;
		tmp.y = y / scalar;

		return tmp;
	}

	Vec2<T>& operator*=(const Vec2<T>& vec)
	{
		x *= vec.x;
		y *= vec.y;

		return *this;
	}

	Vec2<T> operator*(const Vec2<T>& vec) const
	{
		Vec2<T> tmp;

		tmp.x = x * vec.x;
		tmp.y = y * vec.y;

		return tmp;
	}

	Vec2<T>& operator*=(const T scalar)
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}

	Vec2<T> operator*(const T scalar) const
	{
		Vec2<T> tmp;

		tmp.x = x * scalar;
		tmp.y = y * scalar;

		return tmp;
	}

	bool operator<=(const Vec2<T>& other) const
	{
		return x <= other.x && y <= other.y;
	}

	bool operator<(const Vec2<T>& other) const
	{
		return x < other.x && y < other.y;
	}

	bool operator>=(const Vec2<T>& other) const
	{
		return x >= other.x && y >= other.y;
	}

	bool operator>(const Vec2<T>& other) const
	{
		return x > other.x && y > other.y;
	}

	T operator[](const UInt_64 index) const
	{
		switch (index)
		{
			case 0:
				return x;
			case 1:
				return y;
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
			default:
				return x;
		}
	}

	Vec2<T> GetAbs() const
	{
		Vec2<T> tmp;

		tmp.x = Math::Abs(x);
		tmp.y = Math::Abs(y);

		return tmp;
	}

	void Abs()
	{
		x = Math::Abs(x);
		y = Math::Abs(y);
	}

	/// If positive, the vectors are pointing in the same direction. If negative, the vectors are pointing in opposing directions.
	/// If zero, the vectors are perpendicular.
	T GetDot(const Vec2<T>& vec) const
	{
		return x * vec.x + y * vec.y;
	}

	T GetAngle(const Vec2<T>& vec) const
	{
		return Math::ACos(GetDot(vec) / Math::Sqrt(GetMagnitude2() * vec.GetMagnitude2()));
	}

	Vec2<T> GetProjection(const Vec2<T>& length) const
	{
		return operator*(length.GetDot(*this) / GetMagnitude2());
	}

	Vec2<T> GetPerpendicular(const Vec2<T>& length) const
	{
		return length - GetProjection(length);
	}

	Vec2<T> GetReflection(const Vec2<T>& normal) const
	{
		return operator-(normal * (GetDot(normal) * 2));
	}

	T GetMagnitude() const
	{
		return Math::Sqrt(x * x + y * y);
	}

	T GetMagnitude2() const
	{
		return x * x + y * y;
	}

	T GetDistance(const Vec2<T>& vec) const
	{
		return Math::Sqrt(Math::Pow(vec.x - x, 2) + Math::Pow(vec.y - y, 2));
	}

	T GetDistance2(const Vec2<T>& vec) const
	{
		return Math::Pow(vec.x - x, 2) + Math::Pow(vec.y - y, 2);
	}

	Vec2<T> GetNorm() const
	{
		Vec2<T> norm;

		T dis = GetMagnitude();

		norm.x = x / dis;
		norm.y = y / dis;

		return norm;
	}

	void Norm()
	{
		T dis = GetMagnitude();

		x /= dis;
		y /= dis;
	}

	Vec2<T> GetRads() const
	{
		Vec2<T> tmp;

		tmp.x = Math::Rads(x);
		tmp.y = Math::Rads(y);

		return tmp;
	}

	void ToRads()
	{
		x = Math::Rads(x);
		y = Math::Rads(y);
	}

	Vec2<T> GetDegr() const
	{
		Vec2<T> tmp;

		tmp.x = Math::Degr(x);
		tmp.y = Math::Degr(y);

		return tmp;
	}

	void ToDegr()
	{
		x = Math::Degr(x);
		y = Math::Degr(y);
	}

	static Vec2 Lerp(const Vec2& start, const Vec2& finish, const T t)
	{
		return start + (finish - start) * t;
	}
};

typedef Vec2<UInt_64> Vec2_u64;
typedef Vec2<SInt_64> Vec2_s64;
typedef Vec2<Int_64> Vec2_64;
typedef Vec2<UInt_32> Vec2_u32;
typedef Vec2<SInt_32> Vec2_s32;
typedef Vec2<Int_32> Vec2_32;
typedef Vec2<UInt_16> Vec2_u16;
typedef Vec2<SInt_16> Vec2_s16;
typedef Vec2<Int_16> Vec2_16;
typedef Vec2<UInt_8> Vec2_u8;
typedef Vec2<SInt_8> Vec2_s8;
typedef Vec2<Int_8> Vec2_8;
typedef Vec2<float> Vec2_f;
typedef Vec2<double> Vec2_d;