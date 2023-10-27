#pragma once

#include "EHS.h"
#include "UTF.h"
#include "Str.h"
#include "Math.h"
#include "Vec2.h"

template<typename T>
class Vec3
{
public:
	T x;
	T y;
	T z;

	Vec3(const T scalar = 0)
			: x(scalar), y(scalar), z(scalar)
	{
	}

	Vec3(const T x, const T y, const T z)
			: x(x), y(y), z(z)
	{
	}

	template<typename C>
	Vec3(const Vec2<C>& vec, const T z = 0)
		: x((T)vec.x), y((T)vec.y), z(z)
	{
	}

	template<typename C>
	Vec3(const Vec3<C>& vec)
		: x((T)vec.x), y((T)vec.y), z((T)vec.z)
	{
	}

	template<typename C>
	Vec3<T>& operator=(const Vec2<C>& vec)
	{
		x = (T)vec.x;
		y = (T)vec.y;
		z = 0;

		return *this;
	}

	template<typename C>
	Vec3<T>& operator=(const Vec3<C>& vec)
	{
		x = (T)vec.x;
		y = (T)vec.y;
		z = (T)vec.z;

		return *this;
	}

	bool operator==(const Vec3<T>& vec) const
	{
		return Math::ComCmp(x, vec.x) && Math::ComCmp(y, vec.y) && Math::ComCmp(z, vec.z);
	}


	bool operator!=(const Vec3<T>& vec) const
	{
		return !Math::ComCmp(z, vec.z) || !Math::ComCmp(y, vec.y) || !Math::ComCmp(z, vec.z);
	}

	Vec3<T> operator+(const Vec3<T>& vec) const
	{
		Vec3<T> tmp;

		tmp.x = x + vec.x;
		tmp.y = y + vec.y;
		tmp.z = z + vec.z;

		return tmp;
	}

	Vec3<T>& operator+=(const Vec3<T>& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;

		return *this;
	}

	Vec3<T> operator+(const T scalar) const
	{
		Vec3<T> tmp;

		tmp.x = x + scalar;
		tmp.y = y + scalar;
		tmp.z = z + scalar;

		return tmp;
	}

	Vec3<T>& operator+=(const T scalar)
	{
		x += scalar;
		y += scalar;
		z += scalar;

		return *this;
	}

	Vec3<T> operator-(const Vec3<T>& vec) const
	{
		Vec3<T> tmp;

		tmp.x = x - vec.x;
		tmp.y = y - vec.y;
		tmp.z = z - vec.z;

		return tmp;
	}

	Vec3<T>& operator-=(const Vec3<T>& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;

		return *this;
	}

	Vec3<T> operator-(const T scalar) const
	{
		Vec3<T> tmp;

		tmp.x = x - scalar;
		tmp.y = y - scalar;
		tmp.z = z - scalar;

		return tmp;
	}

	Vec3<T>& operator-=(const T scalar)
	{
		x -= scalar;
		y -= scalar;
		z -= scalar;

		return *this;
	}

	Vec3<T> operator*(const Vec3<T>& vec) const
	{
		Vec3<T> tmp;

		tmp.x = x * vec.x;
		tmp.y = y * vec.y;
		tmp.z = z * vec.z;

		return tmp;
	}

	Vec3<T>& operator*=(const Vec3<T>& vec)
	{
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;

		return *this;
	}

	Vec3<T> operator*(const T scalar) const
	{
		Vec3<T> tmp;

		tmp.x = x * scalar;
		tmp.y = y * scalar;
		tmp.z = z * scalar;

		return tmp;
	}

	Vec3<T>& operator*=(const T scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;

		return *this;
	}

	Vec3<T> operator/(const Vec3<T>& vec) const
	{
		Vec3<T> tmp;

		tmp.x = x / vec.x;
		tmp.y = y / vec.y;
		tmp.z = z / vec.z;

		return tmp;
	}

	Vec3<T>& operator/=(const Vec3<T>& vec)
	{
		x /= vec.x;
		y /= vec.y;
		z /= vec.z;

		return *this;
	}

	Vec3<T> operator/(const T scalar) const
	{
		Vec3<T> tmp;

		tmp.x = x / scalar;
		tmp.y = y / scalar;
		tmp.z = z / scalar;

		return tmp;
	}

	Vec3<T>& operator/=(const T scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;

		return *this;
	}

	bool operator<=(const Vec3<T>& other) const
	{
		return x <= other.x && y <= other.y && z <= other.z;
	}

	bool operator<(const Vec3<T>& other) const
	{
		return x < other.x && y < other.y && z < other.z;
	}

	bool operator>=(const Vec3<T>& other) const
	{
		return x >= other.x && y >= other.y && z >= other.z;
	}

	bool operator>(const Vec3<T>& other) const
	{
		return x > other.x && y > other.y && z > other.z;
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
			default:
				return x;
		}
	}

	operator Vec2<T>()
	{
		return Vec2<T>(x, y);
	}

	Vec3<T> GetAbs() const
	{
		Vec3<T> absolute;

		absolute.x = Math::Abs(x);
		absolute.y = Math::Abs(y);
		absolute.z = Math::Abs(z);

		return absolute;
	}

	void Abs()
	{
		x = Math::Abs(x);
		y = Math::Abs(y);
		z = Math::Abs(z);
	}

	Vec3<T> GetNorm() const
	{
		Vec3<T> norm;

		T dis = GetMagnitude();

		norm.x = x / dis;
		norm.y = y / dis;
		norm.z = z / dis;

		return norm;
	}

	void Norm()
	{
		T dis = GetMagnitude();

		x /= dis;
		y /= dis;
		z /= dis;
	}

	Vec3<T> GetCross(const Vec3<T>& vec) const
	{
		return Vec3<T>(
			y * vec.z - z * vec.y,
			z * vec.x - x * vec.z,
			x * vec.y - y * vec.x
		);
	}

	T GetDot(const Vec3<T>& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
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
		return Math::Sqrt(x * x + y * y + z * z);
	}

	T GetMagnitude2() const
	{
		return x * x + y * y + z * z;
	}

	T GetDistance(const Vec3<T>& vec) const
	{
		return (T)Math::Sqrt(Math::Pow<T>(vec.x - x, 2) + Math::Pow<T>(vec.y - y, 2) + Math::Pow<T>(vec.z - z, 2));
	}

	T GetDistance2(const Vec3<T>& vec) const
	{
		return static_cast<T>(Math::Pow<T>(vec.x - x, 2) + Math::Pow<T>(vec.y - y, 2) + Math::Pow<T>(vec.z - z, 2));
	}

	Vec3<T> GetRads() const
	{
		Vec3<T> tmp;

		tmp.x = Math::Rads(x);
		tmp.y = Math::Rads(y);
		tmp.z = Math::Rads(z);

		return tmp;
	}

	void ToRads()
	{
		x = Math::Rads(x);
		y = Math::Rads(y);
		z = Math::Rads(z);
	}

	Vec3<T> GetDegr() const
	{
		Vec3<T> tmp;

		tmp.x = Math::Degr(x);
		tmp.y = Math::Degr(y);
		tmp.z = Math::Degr(z);

		return tmp;
	}

	void ToDegr()
	{
		x = Math::Degr(x);
		y = Math::Degr(y);
		z = Math::Degr(z);
	}

	static Vec3 Lerp(const Vec3& start, const Vec3& finish, const T t)
	{
		return start + (finish - start) * t;
	}
};

typedef Vec3<UInt_64> Vec3_u64;
typedef Vec3<SInt_64> Vec3_s64;
typedef Vec3<Int_64> Vec3_64;
typedef Vec3<UInt_32> Vec3_u32;
typedef Vec3<SInt_32> Vec3_s32;
typedef Vec3<Int_32> Vec3_32;
typedef Vec3<UInt_16> Vec3_u16;
typedef Vec3<SInt_16> Vec3_s16;
typedef Vec3<Int_16> Vec3_16;
typedef Vec3<UInt_8> Vec3_u8;
typedef Vec3<SInt_8> Vec3_s8;
typedef Vec3<Int_8> Vec3_8;
typedef Vec3<float> Vec3_f;
typedef Vec3<double> Vec3_d;