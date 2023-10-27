#pragma once

#include "EHS.h"
#include "Vec3.h"
#include "Mat2.h"

template<typename T = float>
class Mat3
{
private:
	T data[9];

public:
	Mat3()
	{
		for (UInt_8 i = 0; i < 9; ++i)
			data[i] = 0;
	}

	template<typename C>
	Mat3(const Mat2<C>& mat)
	{
		for (UInt_8 i = 0; i < 4; ++i)
			data[i / 2 * 4 + i % 2] = mat.data[i];
	}

	template<typename C>
	Mat3(const Mat3<C>& mat)
	{
		for (UInt_8 i = 0; i < 9; ++i)
			data[i] = mat.data[i];
	}

	template<typename C>
	Mat3<T>& operator=(const Mat3<C>& mat)
	{
		if (this == &mat)
			return *this;

		for (UInt_8 i = 0; i < 9; ++i)
			data[i] = mat.data[i];

		return *this;
	}

	operator Mat2<T>() const
	{
		Mat2<T> result;

		for (UInt_8 i = 0; i < 4; ++i)
			result.data[i] = data[i / 2 * 4 + i % 2];

		return result;
	}

	operator const T*() const
	{
		return data;
	}

	operator T*()
	{
		return data;
	}

	Vec3<T> operator*(Vec3<T> vec) const
	{
		Vec3<T> result;
		result.x = vec.x * data[0] + vec.y * data[3] + vec.z * data[6];
		result.y = vec.x * data[1] + vec.y * data[4] + vec.z * data[7];
		result.z = vec.x * data[2] + vec.y * data[5] + vec.z * data[8];

		return result;
	}

	Mat3<T>& operator*=(const T scalar)
	{
		for (UInt_8 i = 0; i < 9; ++i)
			data[i] *= scalar;

		return *this;
	}

	Mat3<T> operator*(const T scalar) const
	{
		Mat3<T> result;
		for (UInt_8 i = 0; i < 9; ++i)
			result.data[i] = data[i] * scalar;

		return result;
	}

	Mat3<T>& operator*=(const Mat3<T>& mat)
	{
		Mat3<T> old = *this;
		for (UInt_8 i = 0; i < 9; ++i)
		{
			UInt_8 row = i / 3;
			UInt_8 column = i % 3;
			data[i] = 0;
			data[i] += old.data[0 * 3 + column] * mat.data[row * 3 + 0];
			data[i] += old.data[1 * 3 + column] * mat.data[row * 3 + 1];
			data[i] += old.data[2 * 3 + column] * mat.data[row * 3 + 2];
		}

		return *this;
	}

	Mat3<T> operator*(const Mat3<T>& mat) const
	{
		Mat3<T> result;
		for (UInt_8 i = 0; i < 9; ++i)
		{
			UInt_8 row = i / 3;
			UInt_8 column = i % 3;
			result.data[i] += data[0 * 3 + column] * mat.data[row * 3 + 0];
			result.data[i] += data[1 * 3 + column] * mat.data[row * 3 + 1];
			result.data[i] += data[2 * 3 + column] * mat.data[row * 3 + 2];
		}

		return result;
	}

	Mat3<T> GetTranspose() const
	{
		Mat3<T> result;
		for (UInt_8 i = 0; i < 9; ++i)
			result.data[i] = data[3 * (i % 3) + i / 3];

		return result;
	}

	void Transpose()
	{
		Mat3<T> old = *this;
		for (UInt_8 i = 0; i < 9; ++i)
			data[i] = old.data[3 * (i % 3) + i / 3];
	}

	Mat3<T> GetMinor() const
	{
		Mat3<T> result;

		for (UInt_8 r = 0; r < 3; ++r)
			for (UInt_8 c = 0; c < 3; ++c)
				result[3 * r + c] = Cut(r, c).GetDeterminant();

		return result;
	}

	void Minor()
	{
		Mat3<T> old = *this;

		for (UInt_8 r = 0; r < 3; ++r)
			for (UInt_8 c = 0; c < 3; ++c)
				data[3 * r + c] = old.Cut(r, c).GetDeterminant();
	}

	Mat2<T> Cut(const UInt_8 row, const UInt_8 column) const
	{
		Mat2<T> result;
		UInt_8 index = 0;

		for (UInt_8 r = 0; r < 3; ++r)
		{
			for (UInt_8 c = 0; c < 3; ++c)
			{
				if (r == row || c == column)
					continue;

				result[index++] = data[3 * r + c];
			}
		}

		return result;
	}

	T GetDeterminant() const
	{
		Mat3<T> cofactor = GetCofactor();
		T result = 0;

		for (UInt_8 c = 0; c < 3; ++c)
			result += data[c] * cofactor[c];

		return result;
	}

	Mat3<T> GetCofactor() const
	{
		Mat3<T> minor = GetMinor();
		Mat3<T> result;

		for (UInt_8 r = 0; r < 3; ++r)
		{
			for (UInt_8 c = 0; c < 3; ++c)
			{
				UInt_8 i = 3 * c + r;
				result.data[i] = minor.data[i] * Math::Pow<T>(-1, r + c);
			}
		}

		return result;
	}

	void Cofactor()
	{
		Mat3<T> minor = GetMinor();

		for (UInt_8 r = 0; r < 3; ++r)
		{
			for (UInt_8 c = 0; c < 3; ++c)
			{
				UInt_8 i = 3 * c + r;
				data[i] = minor.data[i] * Math::Pow<T>(-1, r + c);
			}
		}
	}

	Mat3<T> GetAdjugate() const
	{
		return GetCofactor().GetTranspose();
	}

	void Adjugate()
	{
		Cofactor();
		Transpose();
	}

	Mat3<T> GetInverse() const
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

	static Mat3<T> Identity()
	{
		Mat3<T> result;
		result.data[0] = 1;
		result.data[4] = 1;
		result.data[8] = 1;
		return result;
	}

	static Mat3<T> Scale(const Vec3<T>& scale)
	{
		Mat3<T> result;
		result.data[0] = scale.x;
		result.data[4] = scale.y;
		result.data[8] = scale.z;

		return result;
	}

	static Mat3<T> PitchRotate(const T angle)
	{
		T radians = Math::Rads(angle);

		Mat3<T> result;
		result.data[0] = 1;
		result.data[4] = Math::Cos(radians);
		result.data[5] = Math::Sin(radians);
		result.data[7] = -Math::Sin(radians);
		result.data[8] = Math::Cos(radians);

		return result;
	}

	static Mat3<T> YawRotate(const T angle)
	{
		T radians = Math::Rads(angle);

		Mat3<T> result;
		result.data[0] = Math::Cos(radians);
		result.data[2] = -Math::Sin(radians);
		result.data[4] = 1;
		result.data[6] = Math::Sin(radians);
		result.data[8] = Math::Cos(radians);

		return result;
	}

	static Mat3<T> RollRotate(const T angle)
	{
		T radians = Math::Rads(angle);

		Mat3<T> result;
		result.data[0] = Math::Cos(radians);
		result.data[1] = Math::Sin(radians);
		result.data[3] = -Math::Sin(radians);
		result.data[4] = Math::Cos(radians);
		result.data[8] = 1;

		return result;
	}

	static Mat3<T> Rotate(const Vec3<T>& vec)
	{
		return YawRotate(vec.y) * RollRotate(vec.z) * PitchRotate(vec.x);
	}
};

typedef Mat3<float> Mat3_f;
typedef Mat3<double> Mat3_d;