#pragma once

#include "EHS.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"

template<typename T>
class Rect
{
public:
	T x;
	T y;
	T w;
	T h;

	Rect(const T scalar = 0)
		: x(scalar), y(scalar), w(scalar), h(scalar)
	{
	}

	Rect(const T x, const T y, const T w, const T h)
			: x(x), y(y), w(w), h(h)
	{
	}

	template<typename C>
	Rect(const Vec2<C>& vec, const T w = 0, const T h = 0)
			: x((T)vec.x), y((T)vec.y), w(w), h(h)
	{
	}

	template<typename C>
	Rect(const Vec3<C>& vec, const T h = 0)
			: x((T)vec.x), y((T)vec.y), w((T)vec.z), h(h)
	{
	}

	template<typename C>
	Rect(const Vec4<C>& vec)
		: x((T)vec.x), y((T)vec.y), w((T)vec.z), h((T)vec.w)
	{
	}

	template<typename C>
	Rect(const Rect<C>& rect)
		: x((T)rect.x), y((T)rect.y), w((T)rect.w), h((T)rect.h)
	{
	}

	template<typename C>
	Rect<T>& operator=(const Vec2<C>& vec)
	{
		x = (T)vec.x;
		y = (T)vec.y;
		w = 0;
		h = 0;

		return *this;
	}

	template<typename C>
	Rect<T>& operator=(const Vec3<C>& vec)
	{
		x = (T)vec.x;
		y = (T)vec.y;
		w = (T)vec.z;
		h = 0;

		return *this;
	}

	template<typename C>
	Rect<T>& operator=(const Vec4<C>& vec)
	{
		x = (T)vec.x;
		y = (T)vec.y;
		w = (T)vec.z;
		h = (T)vec.w;

		return *this;
	}

	template<typename C>
	Rect<T>& operator=(const Rect<C>& rect)
	{
		if (this == &rect)
			return *this;

		x = (T)rect.x;
		y = (T)rect.y;
		w = (T)rect.w;
		h = (T)rect.h;

		return *this;
	}

	bool operator==(const Vec4<T>& vec)
	{
		return x == vec.x && y == vec.y && w == vec.z && h == vec.w;
	}

	bool operator!=(const Vec4<T>& vec)
	{
		return x != vec.x || y != vec.y || w != vec.z || h != vec.w;
	}

	bool operator==(const Rect<T>& rect)
	{
		return x == rect.x && y == rect.y && w == rect.w && h == rect.h;
	}

	bool operator!=(const Rect<T>& rect)
	{
		return x != rect.x || y != rect.y || w != rect.w || h != rect.h;
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
				return w;
			case 3:
				return h;
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
				return w;
			case 3:
				return h;
			default:
				return x;
		}
	}

	operator Vec4<T>()
	{
		return Vec4<T>(x, y, w, h);
	}

	Vec2<T> GetPos() const
	{
		return {x, y};
	}

	Vec2<T> GetScale() const
	{
		return {w, h};
	}
};

typedef Rect<UInt_64> Rect_u64;
typedef Rect<SInt_64> Rect_s64;
typedef Rect<Int_64> Rect_64;
typedef Rect<UInt_32> Rect_u32;
typedef Rect<SInt_32> Rect_s32;
typedef Rect<Int_32> Rect_32;
typedef Rect<UInt_16> Rect_u16;
typedef Rect<SInt_16> Rect_s16;
typedef Rect<Int_16> Rect_16;
typedef Rect<UInt_8> Rect_u8;
typedef Rect<SInt_8> Rect_s8;
typedef Rect<Int_8> Rect_8;
typedef Rect<float> Rect_f;
typedef Rect<double> Rect_d;