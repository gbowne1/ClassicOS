#pragma once

#include "EHS.h"
#include "Math.h"
#include "Mat3.h"
#include "Vec4.h"
#include "Vec3.h"

template <typename T = float>
class Mat4
{
private:
	friend class Uniform;

	T data[16];

public:
	Mat4()
	{
		for (UInt_8 i = 0; i < 16; ++i)
			data[i] = 0;
	}

	explicit Mat4(const T* data)
	{
		for (UInt_8 i = 0; i < 16; ++i)
			this->data[i] = data[i];
	}

	template<typename C>
	Mat4(const Mat3<C>& mat)
	{
		for (UInt_8 i = 0; i < 9; ++i)
		{
			UInt_8 row = i / 3;
			UInt_8 column = i % 3;
			UInt_8 dst = row * 4 + column;

			data[dst] = (T)mat[i];
		}

		data[3] = 0;
		data[7] = 0;
		data[11] = 0;
		data[12] = 0;
		data[13] = 0;
		data[14] = 0;
		data[15] = 1;
	}

	template<typename C>
	Mat4(const Mat4<C>& mat)
	{
		for (UInt_8 i = 0; i < 16; ++i)
			data[i] = (T)mat.data[i];
	}

	template<typename C>
	Mat4<T>& operator=(const Mat4<C>& mat)
	{
		if (this == &mat)
			return *this;

		for (UInt_8 i = 0; i < 16; ++i)
			data[i] = (T)mat.data[i];

		return *this;
	}

	Vec4<T> operator*(Vec4<T> vec) const
	{
		Vec4<T> result;
		result.x = vec.x * data[0] + vec.y * data[4] + vec.z * data[8] + vec.w * data[12];
		result.y = vec.x * data[1] + vec.y * data[5] + vec.z * data[9] + vec.w * data[13];
		result.z = vec.x * data[2] + vec.y * data[6] + vec.z * data[10] + vec.w * data[14];
		result.w = vec.x * data[3] + vec.y * data[7] + vec.z * data[11] + vec.w * data[15];

		return result;
	}

	Mat4<T>& operator*=(const T scalar)
	{
		for (UInt_8 i = 0; i < 16; ++i)
			data[i] *= scalar;

		return *this;
	}

	Mat4<T> operator*(const T scalar) const
	{
		Mat4<T> result;
		for (UInt_8 i = 0; i < 16; ++i)
			result.data[i] = data[i] * scalar;

		return result;
	}

	Mat4<T>& operator*=(const Mat4<T>& mat)
	{
		Mat4<T> old = *this;
		for (UInt_8 i = 0; i < 16; ++i)
		{
			UInt_8 row = i / 4;
			UInt_8 column = i % 4;
			data[i] += old.data[0 * 4 + column] * mat.data[row * 4 + 0];
			data[i] += old.data[1 * 4 + column] * mat.data[row * 4 + 1];
			data[i] += old.data[2 * 4 + column] * mat.data[row * 4 + 2];
			data[i] += old.data[3 * 4 + column] * mat.data[row * 4 + 3];
		}

		return *this;
	}

	Mat4<T> operator*(const Mat4<T>& mat) const
	{
		Mat4<T> result;
		for (UInt_8 i = 0; i < 16; ++i)
		{
			UInt_8 row = i / 4;
			UInt_8 column = i % 4;
			result.data[i] += data[0 * 4 + column] * mat.data[row * 4 + 0];
			result.data[i] += data[1 * 4 + column] * mat.data[row * 4 + 1];
			result.data[i] += data[2 * 4 + column] * mat.data[row * 4 + 2];
			result.data[i] += data[3 * 4 + column] * mat.data[row * 4 + 3];
		}

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

	Vec3<T> GetRight() const
	{
		return Vec3<T>(data[0], data[4], data[8]);
	}

	Vec3<T> GetUp() const
	{
		return Vec3<T>(data[1], data[5], data[9]);
	}

	Vec3<T> GetForward() const
	{
		return Vec3<T>(data[2], data[6], data[10]);
	}

	Mat4<T> GetTranspose() const
	{
		Mat4<T> result;
		for (UInt_8 i = 0; i < 16; ++i)
			result.data[i] = data[4 * (i % 4) + i / 4];

		return result;
	}

	void Transpose()
	{
		Mat4<T> old = *this;
		for (UInt_8 i = 0; i < 16; ++i)
			data[i] = old.data[4 * (i % 4) + i / 4];
	}

	Mat3<T> Cut(const UInt_8 row, const UInt_8 column) const
	{
		Mat3<T> result;
		UInt_8 index = 0;

		for (UInt_8 r = 0; r < 4; ++r)
		{
			for (UInt_8 c = 0; c < 4; ++c)
			{
				if (r == row || c == column)
					continue;

				result[index++] = data[4 * r + c];
			}
		}

		return result;
	}

	Mat4<T> GetMinor() const
	{
		Mat4<T> result;

		for (UInt_8 r = 0; r < 4; ++r)
			for (UInt_8 c = 0; c < 4; ++c)
				result.data[4 * r + c] = Cut(r, c).GetDeterminant();

		return result;
	}

	void Minor()
	{
		Mat4<T> old = *this;

		for (UInt_8 r = 0; r < 4; ++r)
			for (UInt_8 c = 0; c < 4; ++c)
				data[4 * r + c] = old.Cut(r, c).GetDeterminant();
	}

	Mat4<T> GetCofactor() const
	{
		Mat4<T> minor = GetMinor();
		Mat4<T> result;

		for (UInt_8 r = 0; r < 4; ++r)
		{
			for (UInt_8 c = 0; c < 4; ++c)
			{
				UInt_8 i = 4 * c + r;
				result.data[i] = minor.data[i] * Math::Pow<T>(-1, r + c);
			}
		}

		return result;
	}

	void Cofactor()
	{
		Mat4<T> minor = GetMinor();

		for (UInt_8 r = 0; r < 4; ++r)
		{
			for (UInt_8 c = 0; c < 4; ++c)
			{
				UInt_8 i = 4 * c + r;
				data[i] = minor.data[i] * Math::Pow<T>(-1, r + c);
			}
		}
	}

	T GetDeterminant() const
	{
		Mat4<T> cofactor = GetCofactor();
		T result = 0;

		for (UInt_8 c = 0; c < 4; ++c)
			result += data[c] * cofactor[c];

		return result;
	}

	Mat4<T> GetAdjugate() const
	{
		return GetCofactor().GetTranspose();
	}

	void Adjugate()
	{
		Cofactor();
		Transpose();
	}

	Mat4<T> GetInverse() const
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

	static Mat4<T> Identity()
	{
		Mat4<T> result;
		result.data[0] = 1;
		result.data[5] = 1;
		result.data[10] = 1;
		result.data[15] = 1;
		return result;
	}

	static Mat4<T> Scale(const Vec3<T>& scale)
	{
		Mat4<T> result;
		result.data[0] = scale.x;
		result.data[5] = scale.y;
		result.data[10] = scale.z;
		result.data[15] = 1;

		return result;
	}

	static Mat4<T> Translate(const Vec3<T>& pos)
	{
		Mat4<T> result = Identity();
		result.data[12] = pos.x;
		result.data[13] = pos.y;
		result.data[14] = pos.z;

		return result;
	}

	static Mat4<T> PitchRotate(const T angle)
	{
		T radians = Math::Rads(angle);

		Mat4<T> result;
		result.data[0] = 1;
		result.data[5] = Math::Cos(radians);
		result.data[6] = Math::Sin(radians);
		result.data[9] = -Math::Sin(radians);
		result.data[10] = Math::Cos(radians);
		result.data[15] = 1;

		return result;
	}

	static Mat4<T> YawRotate(const T angle)
	{
		T radians = Math::Rads(angle);

		Mat4<T> result;
		result.data[0] = Math::Cos(radians);
		result.data[2] = -Math::Sin(radians);
		result.data[5] = 1;
		result.data[8] = Math::Sin(radians);
		result.data[10] = Math::Cos(radians);
		result.data[15] = 1;

		return result;
	}

	static Mat4<T> RollRotate(const T angle)
	{
		T radians = Math::Rads(angle);

		Mat4<T> result;
		result.data[0] = Math::Cos(radians);
		result.data[1] = Math::Sin(radians);
		result.data[4] = -Math::Sin(radians);
		result.data[5] = Math::Cos(radians);
		result.data[10] = 1;
		result.data[15] = 1;

		return result;
	}

	static Mat4<T> Rotate(const Vec3<T>& vec)
	{
		return YawRotate(vec.y) * RollRotate(vec.z) * PitchRotate(vec.x);
	}

	static Mat4<T> RH_Perspective(const T fov, const T aspect, const T zNear, const T zFar)
	{
		const float tanHalfFovy = tan(Math::Rads(fov) / 2.0f);

		Mat4<T> result;
		result[0] = 1.0f / (aspect * tanHalfFovy);
		result[5] = -1.0f / tanHalfFovy;
		result[10] = zFar / (zFar - zNear);
		result[14] = -(zFar * zNear) / (zFar - zNear);

		return result;
	}

	static Mat4<T> LH_Perspective(const T fov, const T aspect, const T zNear, const T zFar)
	{
		const float tanHalfFovy = Math::Tan(Math::Rads(fov) / 2.0f);

		Mat4<T> result;
		result[0] = 1.0f / (aspect * tanHalfFovy);
		result[5] = -1.0f / tanHalfFovy;
		result[10] = zFar / (zFar - zNear);
		result[11] = 1.0f;
		result[14] = -(zFar * zNear) / (zFar - zNear);
		result[15] = 0.0f;

		return result;
	}

	static Mat4<T> LH_Orthographic(const T left, const T right, const T top, const T bottom, const T zNear, const T zFar)
	{
		Mat4<T> result;
		result[0] = 2.0f / (right - left); // 0,0 entry
		result[5] = 2.0f / (bottom - top); // 1,1 entry
		result[10] = 1.0f / (zFar - zNear); // 2,2 entry
		result[12] = -(right + left) / (right - left); // 3,0 entry
		result[13] = -(bottom + top) / (bottom - top); // 3,1 entry
		result[14] = -zNear / (zFar - zNear); // 3,2 entry
		result[15] = 1.0f; // 3,3 entry

		return result;

		/*
		Mat4<T> result;
		result.data[0] = 2 / (right - left);
		result.data[5] = 2 / (top - bottom);
		result.data[10] = 1 / (zFar - zNear);
		result.data[12] = (left + right) / (left - right);
		result.data[13] = (top + bottom) / (bottom - top);
		result.data[14] = zNear / (zNear - zFar);
		result.data[15] = 1;

		return result;
		*/
	}
};

typedef Mat4<float> Mat4_f;
typedef Mat4<double> Mat4_d;