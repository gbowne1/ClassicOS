#pragma once

#include "EHS.h"
#include "Vec2.h"

template<typename T = float>
class Mat2
{
private:
	T data[4];

public:
	Mat2()
	{
		for (UInt_8 i = 0; i < 4; ++i)
			data[i] = 0;
	}

	template<typename C>
	Mat2(const Mat2<C>& mat)
	{
		for (UInt_8 i = 0; i < 4; ++i)
			data[i] = mat.data[i];
	}

	template<typename C>
	Mat2<T>& operator=(const Mat2<C>& mat)
	{
		if (this == &mat)
			return *this;

		for (UInt_8 i = 0; i < 4; ++i)
			data[i] = mat.data[i];

		return *this;
	}

	operator const T*() const
	{
		return data;
	}

	operator T*()
	{
		return data;
	}

	Vec2<T> operator*(Vec2<T> vec) const
	{
		Vec2<T> result;
		result.x = vec.x * data[0] + vec.y * data[2];
		result.y = vec.x * data[1] + vec.y * data[3];

		return result;
	}

	Mat2<T>& operator*=(const T scalar)
	{
		for (UInt_8 i = 0; i < 4; ++i)
			data[i] *= scalar;

		return *this;
	}

	Mat2<T> operator*(const T scalar) const
	{
		Mat2<T> result;
		for (UInt_8 i = 0; i < 4; ++i)
			result.data[i] = data[i] * scalar;

		return result;
	}

	Mat2<T>& operator*=(const Mat2<T>& mat)
	{
		Mat2<T> old = *this;
		for (UInt_8 i = 0; i < 4; ++i)
		{
			UInt_8 row = i / 2;
			UInt_8 column = i % 2;
			data[i] = 0;
			data[i] += old.data[0 * 2 + column] * mat.data[row * 2 + 0];
			data[i] += old.data[1 * 2 + column] * mat.data[row * 2 + 1];
		}

		return *this;
	}

	Mat2<T> operator*(const Mat2<T>& mat) const
	{
		Mat2<T> result;
		for (UInt_8 i = 0; i < 4; ++i)
		{
			UInt_8 row = i / 2;
			UInt_8 column = i % 2;
			result.data[i] += data[0 * 2 + column] * mat.data[row * 2 + 0];
			result.data[i] += data[1 * 2 + column] * mat.data[row * 2 + 1];
		}

		return result;
	}

	Mat2<T> GetTranspose() const
	{
		Mat2<T> result;
		for (UInt_8 i = 0; i < 4; ++i)
			result.data[i] = data[2 * (i % 2) + i / 2];

		return result;
	}

	void Transpose()
	{
		Mat2<T> old = *this;
		for (UInt_8 i = 0; i < 4; ++i)
			data[i] = old.data[2 * (i % 2) + i / 2];
	}

	Mat2<T> GetMinor() const
	{
		Mat2<T> result(0);
		result.data[0] = data[3];
		result.data[1] = data[2];
		result.data[2] = data[1];
		result.data[3] = data[0];
		return result;
	}

	void Minor()
	{
		Mat2<T> old = *this;
		data[0] = old.data[3];
		data[1] = old.data[2];
		data[2] = old.data[1];
		data[3] = old.data[0];
	}

	T GetDeterminant() const
	{
		return data[0] * data[3] - data[1] * data[2];
	}

	Mat2<T> GetCofactor() const
	{
		Mat2<T> minor = GetMinor();
		Mat2<T> result;

		for (UInt_8 r = 0; r < 2; ++r)
		{
			for (UInt_8 c = 0; c < 2; ++c)
			{
				UInt_8 i = 2 * c + r;
				result.data[i] = minor.data[i] * Math::Pow<T>(-1, r + c);
			}
		}

		return result;
	}

	void Cofactor()
	{
		Mat2<T> minor = GetMinor();

		for (UInt_8 r = 0; r < 2; ++r)
		{
			for (UInt_8 c = 0; c < 2; ++c)
			{
				UInt_8 i = 2 * c + r;
				data[i] = minor.data[i] * Math::Pow<T>(-1, r + c);
			}
		}
	}

	Mat2<T> GetAdjugate() const
	{
		return GetCofactor().GetTranspose();
	}

	void Adjugate()
	{
		Cofactor();
		Transpose();
	}

	Mat2<T> GetInverse() const
	{
		T det = GetDeterminant();
		if (Math::ComCmp(det, 0.0f))
			return {};

		return GetAdjugate() * (1 / det);
	}

	void Inverse()
	{
		T det = GetDeterminant();
		if (Math::ComCmp(det, 0.0f))
			return;

		Adjugate();
		operator*=(1 / det);
	}

	static Mat2<T> Identity()
	{
		Mat2<T> result;
		result[0] = 1;
		result[3] = 1;
		return result;
	}
};

typedef Mat2<float> Mat2_f;
typedef Mat2<double> Mat2_d;