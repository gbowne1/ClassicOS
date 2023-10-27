#pragma once

#include "EHS.h"
#include "Math.h"
#include "Mat4.h"
#include "Vec3.h"

template<typename T>
class Quat
{
public:
	T w;
	T x;
	T y;
	T z;

	Quat()
		: w(0), x(0), y(0), z(0)
	{
	}

	Quat(const T w, const T x, const T y, const T z)
			: w(w), x(x), y(y), z(z)
	{
	}

	Quat(const T yaw, const T pitch, const T roll)
			: w(0), x(0), y(0), z(0)
	{
		T c1 = cos(yaw / 2);
		T c2 = cos(pitch / 2);
		T c3 = cos(roll / 2);
		T s1 = sin(yaw / 2);
		T s2 = sin(pitch / 2);
		T s3 = sin(roll / 2);

		w = c1 * c2 * c3 - s1 * s2 * s3;
		x = s1 * s2 * c3 + c1 * c2 * s3;
		y = s1 * c2 * c3 + c1 * s2 * s3;
		z = c1 * s2 * c3 - s1 * c2 * s3;
	}

	explicit Quat(const Vec3<T>& euler)
			: w(0), x(0), y(0), z(0)
	{
		T c1 = cos(euler.x / 2);
		T c2 = cos(euler.y / 2);
		T c3 = cos(euler.z / 2);
		T s1 = sin(euler.x / 2);
		T s2 = sin(euler.y / 2);
		T s3 = sin(euler.z / 2);

		w = c1 * c2 * c3 - s1 * s2 * s3;
		x = s1 * s2 * c3 + c1 * c2 * s3;
		y = s1 * c2 * c3 + c1 * s2 * s3;
		z = c1 * s2 * c3 - s1 * c2 * s3;
	}

	Quat(const Vec3<T>& n, const T a)
			: w(cosf(a / 2)), x(n.x * sinf(a / 2)), y(n.y * sinf(a / 2)), z(n.z * sinf(a / 2))
	{
	}

	explicit Quat(const Mat4<T>& rotMatrix)
			: w(0), x(0), y(0), z(0)
	{
		ToQuaternion(rotMatrix);
	}

	Quat(const Quat& quat)
			: w(quat.w), x(quat.x), y(quat.y), z(quat.z)
	{
	}

	explicit Quat(const T scalar)
			: w(scalar), x(scalar), y(scalar), z(scalar)
	{
	}

	Quat& operator=(const Quat& quat)
	{
		if (this == &quat)
			return *this;

		w = quat.w;
		x = quat.x;
		y = quat.y;
		z = quat.z;

		return *this;
	}

	Quat& operator=(const Mat4<T>& rotMatrix)
	{
		ToQuaternion(rotMatrix);

		return *this;
	}

	Quat operator+(const Quat& other) const
	{
		return {w + other.w, x + other.x, y + other.y, z + other.z};
	}

	Quat operator-() const
	{
		return {-w, -x, -y, -z};
	}

	Quat operator-(const Quat& other) const
	{
		return {w - other.w, x - other.x, y - other.y, z - other.z};
	}

	Quat operator*(const T scalar)
	{
		return {w * scalar, x * scalar, x * scalar, x * scalar};
	}

	Quat operator*(const Quat& other)
	{
		return Quat
		(
			w * other.w - x * other.x - y * other.y - z * other.z,
			w * other.x + x * other.w + y * other.z - z * other.y,
			w * other.y - x * other.z + y * other.w + z * other.x,
			w * other.z + x * other.y - y * other.x + z * other.w
		);
	}

	Vec3<T> operator*(const Vec3<T>& vect)
	{
		Quat tmp(0, vect[0], vect[1], vect[2]);
		Vec3<T> tmpVect(x, y, z);

		Vec3<T> vcV = tmpVect.CrossProduct(vect);
		return vect + vcV * (2 * w) + tmpVect.CrossProduct(vcV) * 2;
	}

	Quat operator^(const T t)
	{
		Vec3<T> n;
		T a;

		ToAxisAngle(&n, &a);

		float at = a * t;

		return Quat<T>(n, at);
	}

	bool operator==(const Quat& quat) const
	{
		return w == quat.w && x == quat.x && y == quat.y && z == quat.z;
	}

	bool operator!=(const Quat& quat) const
	{
		return w != quat.w || x != quat.x || y != quat.y || z == quat.z;
	}

	T operator[](const UInt_64 index) const
	{
		switch (index)
		{
			case 0:
				return w;
			case 1:
				return x;
			case 2:
				return y;
			case 3:
				return z;
			default:
				return w;
		}
	}

	T& operator[](const UInt_64 index)
	{
		switch (index)
		{
			case 0:
				return w;
			case 1:
				return x;
			case 2:
				return y;
			case 3:
				return z;
			default:
				return w;
		}
	}

	void ToAxisAngle(Vec3<T>* vectAxis, T* flAngle)
	{
		Vec3<T> tmp(x, y, z);

		if (tmp.GetDis2() < 0.0001f)
			*vectAxis = Vec3<T>(1, 0, 0);
		else
			*vectAxis = tmp.GetNorm();

		*flAngle = acosf(w) * 2;
		*flAngle = Math::Degr<T>(*flAngle);
	}

	void ToQuaternion(const Mat4<T>& rotMatrix)
	{
		T trace = rotMatrix[0][0] + rotMatrix[1][1] + rotMatrix[2][2];

		if (trace > 0)
		{
			T s = 0.5f / Math::Sqrt<T>(trace + 1.0f);
			w = 0.25f / s;

			x = (rotMatrix[2][1] - rotMatrix[1][2]) * s;
			y = (rotMatrix[0][2] - rotMatrix[2][0]) * s;
			z = (rotMatrix[1][0] - rotMatrix[0][1]) * s;
		} else
		{
			if ((rotMatrix[0][0] > rotMatrix[1][1]) && (rotMatrix[0][0] > rotMatrix[2][2]))
			{
				T s = 2.0f * Math::Sqrt(1.0f + rotMatrix[0][0] - rotMatrix[1][1] - rotMatrix[2][2]);
				w = (rotMatrix[2][1] - rotMatrix[1][2]) / s;
				x = 0.25f * s;
				y = (rotMatrix[0][1] + rotMatrix[1][0]) / s;
				z = (rotMatrix[0][2] + rotMatrix[2][0]) / s;
			} else if (rotMatrix[1][1] > rotMatrix[2][2])
			{
				T s = 2.0f * sqrtf(1.0f + rotMatrix[1][1] - rotMatrix[0][0] - rotMatrix[2][2]);
				w = (rotMatrix[0][2] - rotMatrix[2][0]) / s;
				x = (rotMatrix[0][1] + rotMatrix[1][0]) / s;
				y = 0.25f * s;
				z = (rotMatrix[1][2] + rotMatrix[2][1]) / s;
			} else
			{
				T s = 2.0f * sqrtf(1.0f + rotMatrix[2][2] - rotMatrix[0][0] - rotMatrix[1][1]);
				w = (rotMatrix[1][0] - rotMatrix[0][1]) / s;
				x = (rotMatrix[0][2] + rotMatrix[2][0]) / s;
				y = (rotMatrix[1][2] + rotMatrix[2][1]) / s;
				z = 0.25f * s;
			}
		}
	}

	/*
	Vec3<T> ToEulerAngle() const
	{
		Vec3<T> euler;

		float ysqr = y * y;

		float t0 = 2 * (w * x + y * z);
		float t1 = 1 - 2 * (x * x + ysqr);
		euler.z = std::atan2(t0, t1);

		float t2 = 2 * (w * y - z * x);
		t2 = t2 > 1 ? 1 : t2;
		t2 = t2 < -1 ? -1 : t2;
		euler.y = std::asin(t2);

		float t3 = 2 * (w * z + x * y);
		float t4 = 1 - 2 * (ysqr + z * z);
		euler.x = std::atan2(t3, t4);

		return euler;
	}
	*/

	Mat4<T> ToMatrix() const
	{
		Mat4<T> result;

		T x2 = x + x;
		T y2 = y + y;
		T z2 = z + z;
		T x2w = x2 * w;
		T y2w = y2 * w;
		T z2w = z2 * w;
		T x2x = x2 * x;
		T y2x = y2 * x;
		T z2x = z2 * x;
		T y2y = y2 * y;
		T z2y = z2 * y;
		T z2z = z2 * y;

		result[0] = T(1) - (y2y + z2z);
		result[1] = y2x - z2w;
		result[2] = z2x + y2w;
		result[3] = T(0);

		result[4] = y2x + z2w;
		result[5] = T(1) - (x2x + z2z);
		result[6] = z2y - x2w;
		result[7] = T(0);

		result[8] = z2x - y2w;
		result[9] = z2y + x2w;
		result[10] = T(1) - (x2x + y2y);
		result[11] = T(0);

		result[12] = T(0);
		result[13] = T(0);
		result[14] = T(0);
		result[15] = T(1);

		return result;
	}

	float GetMagnitude()
	{
		return Math::Sqrt<T>(Math::Pow<T>(w, 2) + Math::Pow<T>(x, 2) + Math::Pow<T>(y, 2) + Math::Pow<T>(z, 2));
	}

	Quat<T> GetNormalized()
	{
		T mag = GetMagnitude();

		return Quat<T>(w / mag, x / mag, y / mag, z / mag);
	}

	void Normalize()
	{
		T mag = GetMagnitude();

		w = w / mag;
		x = x / mag;
		y = y / mag;
		z = z / mag;
	}

	T Dot(const Quat& other) const
	{
		return w * other.w + x * other.x + y * other.y + z * other.z;
	}

	Quat<T> GetConjugate()
	{
		return Quat<T>(w, -x, -y, -z);
	}

	void Conjugate()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	Quat<T> GetInverse()
	{
		return Quat<T>(w, -x, -y, -z);
	}

	void Inverse()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	static Quat<T> Slerp(Quat<T> start, Quat<T> finish, const T t)
	{
		T cosHalfTheta = start.Dot(finish);
		if (Math::Abs(cosHalfTheta) >= 1.0f)
			return start;

		float halfTheta = Math::ACos(cosHalfTheta);
		float sinHalfTheta = Math::Sqrt(1.0f - cosHalfTheta * cosHalfTheta);
		if (Math::Abs(sinHalfTheta) < 0.001f)
		{
			return {
				start.w * 0.5f + finish.w * 0.5f,
				start.x * 0.5f + finish.x * 0.5f,
				start.y * 0.5f + finish.y * 0.5f,
				start.z * 0.5f + finish.z * 0.5f
			};
		}

		float ratioA = Math::Sin((1 - t) * halfTheta) / sinHalfTheta;
		float ratioB = Math::Sin(t * halfTheta) / sinHalfTheta;

		return {
			start.w * ratioA + finish.w * ratioB,
			start.x * ratioA + finish.x * ratioB,
			start.y * ratioA + finish.y * ratioB,
			start.z * ratioA + finish.z * ratioB
		};
	}
};

typedef Quat<float> Quat_f;