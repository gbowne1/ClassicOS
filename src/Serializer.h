#pragma once

#include "EHS.h"
#include "Util.h"
#include "Array.h"
#include "Vector.h"
#include "Str.h"
#include "Version.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Color3.h"
#include "Vec4.h"
#include "Rect.h"
#include "Color4.h"
#include "Quat.h"
#include "Mat2.h"
#include "Mat3.h"
#include "Mat4.h"

template<typename N = UInt_64>
class Serializer
{
private:
	Endianness endianness;
	Byte* data;
	N size;
	N offset;

public:
	~Serializer()
	{
		delete[] data;
	}

	Serializer()
		: endianness(Endianness::BE), data(nullptr), size(0), offset(0)
	{
	}

	Serializer(const Endianness endianness)
		: endianness(endianness), data(nullptr), size(0), offset(0)
	{
	}

	Serializer(const Endianness endianness, const N size)
		: endianness(endianness), data(new Byte[size]), size(size), offset(0)
	{
	}

	Serializer(const Endianness endianness, const Byte* const data, const N size, const N offset = 0)
		: endianness(endianness), data(new Byte[size]), size(size), offset(offset)
	{
		for (N i = 0; i < size; ++i)
			this->data[i] = data[i];
	}

	Serializer(const Serializer& serializer)
		: endianness(serializer.endianness), data(new Byte[serializer.size]), size(serializer.size), offset(serializer.offset)
	{
		for (N i = 0; i < serializer.size; ++i)
			data[i] = serializer.data[i];
	}

	Serializer(Serializer&& serializer) noexcept
		: endianness(serializer.endianness), data(serializer.data), size(serializer.size), offset(serializer.offset)
	{
		serializer.data = nullptr;
		serializer.size = 0;
		serializer.offset = 0;
	}

	Serializer& operator=(const Serializer& serializer)
	{
		if (this == &serializer)
			return *this;

		endianness = serializer.endianness;

		delete[] data;
		data = new Byte[serializer.size];
		for (N i = 0; i < serializer.size; ++i)
			data[i] = serializer.data[i];

		size = serializer.size;
		offset = serializer.offset;

		return *this;
	}

	Serializer& operator=(Serializer&& serializer) noexcept
	{
		if (this == &serializer)
			return *this;

		endianness = serializer.endianness;
		delete[] data;
		data = serializer.data;
		size = serializer.size;
		offset = serializer.offset;

		serializer.data = nullptr;
		serializer.size = 0;
		serializer.offset = 0;

		return *this;
	}

	Serializer& operator+=(const N size)
	{
		offset += size;
		return *this;
	}

	Serializer& operator-=(const N size)
	{
		offset -= size;
		return *this;
	}

	Serializer& operator*=(const N size)
	{
		offset *= size;
		return *this;
	}

	Serializer& operator/=(const N size)
	{
		offset /= size;
		return *this;
	}

	Serializer& operator%=(const N size)
	{
		offset %= size;
		return *this;
	}

	operator const Byte* () const
	{
		return data;
	}

	operator Byte* ()
	{
		return data;
	}

	void SetEndianness(const Endianness newEndianness)
	{
		endianness = newEndianness;
	}

	Endianness GetEndianness() const
	{
		return endianness;
	}

	template<typename T, typename O = UInt_64>
	void WriteArray(const T* const value, const O size)
	{
		if (sizeof(N) + size * sizeof(T) > this->size - offset)
		{
			N remainder = sizeof(N) + size * sizeof(T) - (this->size - offset);

			Byte* r = new Byte[this->size + remainder];

			for (N i = 0; i < this->size; ++i)
				r[i] = data[i];

			delete[] data;
			data = r;
			this->size += remainder;
		}

		if (CPU::GetEndianness() == Endianness::LE)
		{
			if (endianness == Endianness::LE)
			{
				*(N*)&data[offset] = (N)size;

				offset += sizeof(N);

				for (N i = 0; i < size; ++i)
					*(T*)&data[offset + i * sizeof(T)] = value[i];
			}
			else
			{
				N tmp = (N)size;

				for (N i = 0; i < sizeof(N); ++i)
					data[offset + i] = ((Byte*)&tmp)[sizeof(N) - i - 1];

				offset += sizeof(N);

				for (N i = 0; i < size; ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = ((Byte*)&value[i])[sizeof(T) - i - 1];
			}
		}
		else
		{
			if (endianness == Endianness::LE)
			{
				N tmp = (N)size;

				for (N i = 0; i < sizeof(N); ++i)
					data[offset + i] = ((Byte*)&tmp)[sizeof(N) - i - 1];

				offset += sizeof(N);

				for (N i = 0; i < size; ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = ((Byte*)&value[i])[sizeof(T) - i - 1];
			}
			else
			{
				*(N*)&data[offset] = (N)size;

				offset += sizeof(N);

				for (N i = 0; i < size; ++i)
					*(T*)&data[offset + i * sizeof(T)] = value[i];
			}
		}

		offset += sizeof(T) * size;
	}

	template<typename T, typename O = UInt_64>
	void WriteArray(const Array<T, O>& value)
	{
		if (sizeof(N) + value.Size() * sizeof(T) > size - offset)
		{
			N remainder = sizeof(N) + value.Size() * sizeof(T) - (size - offset);

			Byte* r = new Byte[size + remainder];

			for (N i = 0; i < size; ++i)
				r[i] = data[i];

			delete[] data;
			data = r;
			size += remainder;
		}

		if (CPU::GetEndianness() == Endianness::LE)
		{
			if (endianness == Endianness::LE)
			{
				*(N*)&data[offset] = (N)value.Size();

				offset += sizeof(N);

				for (N i = 0; i < value.Size(); ++i)
					*(T*)&data[offset + i * sizeof(T)] = value[i];
			}
			else
			{
				N tmpSize = (N)value.Size();

				for (N i = 0; i < sizeof(N); ++i)
					data[offset + i] = ((Byte*)&tmpSize)[sizeof(N) - i - 1];

				offset += sizeof(N);

				for (N i = 0; i < value.Size(); ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = ((Byte*)&value[i])[sizeof(T) - i - 1];
			}
		}
		else
		{
			if (endianness == Endianness::LE)
			{
				N tmpSize = (N)value.Size();

				for (N i = 0; i < sizeof(N); ++i)
					data[offset + i] = ((Byte*)&tmpSize)[sizeof(N) - i - 1];

				offset += sizeof(N);

				for (N i = 0; i < value.Size(); ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = ((Byte*)&value[i])[sizeof(T) - i - 1];
			}
			else
			{
				*(N*)&data[offset] = (N)value.Size();

				offset += sizeof(N);

				for (N i = 0; i < value.Size(); ++i)
					*(T*)&data[offset + i * sizeof(T)] = value[i];
			}
		}

		offset += sizeof(T) * value.Size();
	}

	template<typename T, typename O = UInt_64>
	void WriteVector(const Vector<T, O>& value)
	{
		if (sizeof(N) + value.Size() * sizeof(T) > size - offset)
		{
			N remainder = sizeof(N) + value.Size() * sizeof(T) - (size - offset);

			Byte* r = new Byte[size + remainder];

			for (N i = 0; i < size; ++i)
				r[i] = data[i];

			delete[] data;
			data = r;
			size += remainder;
		}

		if (CPU::GetEndianness() == Endianness::LE)
		{
			if (endianness == Endianness::LE)
			{
				*(N*)&data[offset] = (N)value.Size();

				offset += sizeof(N);

				for (N i = 0; i < value.Size(); ++i)
					*(T*)&data[offset + i * sizeof(T)] = value[i];
			}
			else
			{
				N tmpSize = (N)value.Size();

				for (N i = 0; i < sizeof(N); ++i)
					data[offset + i] = ((Byte*)&tmpSize)[sizeof(N) - i - 1];

				offset += sizeof(N);

				for (N i = 0; i < value.Size(); ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = ((Byte*)&value[i])[sizeof(T) - i - 1];
			}
		}
		else
		{
			if (endianness == Endianness::LE)
			{
				N tmpSize = (N)value.Size();

				for (N i = 0; i < sizeof(N); ++i)
					data[offset + i] = ((Byte*)&tmpSize)[sizeof(N) - i - 1];

				offset += sizeof(N);

				for (N i = 0; i < value.Size(); ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = ((Byte*)&value[i])[sizeof(T) - i - 1];
			}
			else
			{
				*(N*)&data[offset] = (N)value.Size();

				offset += sizeof(N);

				for (N i = 0; i < value.Size(); ++i)
					*(T*)&data[offset + i * sizeof(T)] = value[i];
			}
		}

		offset += sizeof(T) * value.Size();
	}

	template<typename T, typename O = UInt_64>
	void WriteStr(const Str<T, O>& str)
	{
		if (sizeof(N) + sizeof(T) * str.Size() > size - offset)
		{
			N remainder = sizeof(N) + sizeof(T) * str.Size() - (size - offset);

			Byte* r = new Byte[size + remainder];

			for (N i = 0; i < size; ++i)
				r[i] = data[i];

			delete[] data;
			data = r;
			size += remainder;
		}

		if (CPU::GetEndianness() == Endianness::LE)
		{
			if (endianness == Endianness::LE)
			{
				*(N*)&data[offset] = (N)str.Size();

				offset += sizeof(N);

				for (N i = 0; i < str.Size(); ++i)
					*(T*)&data[offset + i * sizeof(T)] = str[i];
			}
			else
			{
				N tmpSize = (N)str.Size();

				for (N i = 0; i < sizeof(N); ++i)
					data[offset + i] = ((Byte*)&tmpSize)[sizeof(N) - i - 1];

				offset += sizeof(N);

				for (N i = 0; i < str.Size(); ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = ((Byte*)&str[i])[sizeof(T) - i - 1];
			}
		}
		else
		{
			if (endianness == Endianness::LE)
			{
				N tmpSize = (N)str.Size();

				for (N i = 0; i < sizeof(N); ++i)
					data[offset + i] = ((Byte*)&tmpSize)[sizeof(N) - i - 1];

				offset += sizeof(N);

				for (N i = 0; i < str.Size(); ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = ((Byte*)&str[i])[sizeof(T) - i - 1];
			}
			else
			{
				*(N*)&data[offset] = (N)str.Size();

				offset += sizeof(N);

				for (N i = 0; i < str.Size(); ++i)
					*(T*)&data[offset + i * sizeof(T)] = str[i];
			}
		}

		offset += str.Size() * sizeof(T);
	}

	void WriteVersion(const Version& value)
	{
		if (sizeof(unsigned int) * 3 > size - offset)
		{
			N remainder = sizeof(unsigned int) * 3 - (size - offset);

			Byte* r = new Byte[size + remainder];

			for (N i = 0; i < size; ++i)
				r[i] = data[i];

			delete[] data;
			data = r;
			size += remainder;
		}

		if (CPU::GetEndianness() == Endianness::LE)
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < 3; ++i)
					*(unsigned int*)&data[offset + i * sizeof(unsigned int)] = value[i];
			else
				for (N i = 0; i < 3; ++i)
					for (N b = 0; b < sizeof(unsigned int); ++b)
						data[offset + i * sizeof(unsigned int) + b] = value[sizeof(unsigned int) - i - 1];
		}
		else
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < 3; ++i)
					for (N b = 0; b < sizeof(unsigned int); ++b)
						data[offset + i * sizeof(unsigned int) + b] = value[sizeof(unsigned int) - i - 1];
			else
				for (N i = 0; i < 3; ++i)
					*(unsigned int*)&data[offset + i * sizeof(unsigned int)] = value[i];
		}

		offset += sizeof(unsigned int) * 3;
	}

	template<typename T>
	void WriteVec2(const Vec2<T>& value)
	{
		if (sizeof(T) * 2 > size - offset)
		{
			N remainder = sizeof(T) * 2 - (size - offset);

			Byte* r = new Byte[size + remainder];

			for (N i = 0; i < size; ++i)
				r[i] = data[i];

			delete[] data;
			data = r;
			size += remainder;
		}

		if (CPU::GetEndianness() == Endianness::LE)
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < 2; ++i)
					*(T*)&data[offset + i * sizeof(T)] = value[i];
			else
				for (N i = 0; i < 2; ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = value[sizeof(T) - i - 1];
		}
		else
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < 2; ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = value[sizeof(T) - i - 1];
			else
				for (N i = 0; i < 2; ++i)
					*(T*)&data[offset + i * sizeof(T)] = value[i];
		}

		offset += sizeof(T) * 2;
	}

	template<typename T>
	void WriteVec3(const Vec3<T>& value)
	{
		if (sizeof(T) * 3 > size - offset)
		{
			N remainder = sizeof(T) * 3 - (size - offset);

			Byte* r = new Byte[size + remainder];

			for (N i = 0; i < size; ++i)
				r[i] = data[i];

			delete[] data;
			data = r;
			size += remainder;
		}

		if (CPU::GetEndianness() == Endianness::LE)
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < 3; ++i)
					*(T*)&data[offset + i * sizeof(T)] = value[i];
			else
				for (N i = 0; i < 3; ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = value[sizeof(T) - i - 1];
		}
		else
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < 3; ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = value[sizeof(T) - i - 1];
			else
				for (N i = 0; i < 3; ++i)
					*(T*)&data[offset + i * sizeof(T)] = value[i];
		}

		offset += sizeof(T) * 3;
	}

	void WriteColor3(const Color3& value)
	{
		if (sizeof(float) * 3 > size - offset)
		{
			N remainder = sizeof(float) * 3 - (size - offset);

			Byte* r = new Byte[size + remainder];

			for (N i = 0; i < size; ++i)
				r[i] = data[i];

			delete[] data;
			data = r;
			size += remainder;
		}

		if (CPU::GetEndianness() == Endianness::LE)
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < 3; ++i)
					*(float*)&data[offset + i * sizeof(float)] = value[i];
			else
				for (N i = 0; i < 3; ++i)
					for (N b = 0; b < sizeof(float); ++b)
						data[offset + i * sizeof(float) + b] = value[sizeof(float) - i - 1];
		}
		else
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < 3; ++i)
					for (N b = 0; b < sizeof(float); ++b)
						data[offset + i * sizeof(float) + b] = value[sizeof(float) - i - 1];
			else
				for (N i = 0; i < 3; ++i)
					*(float*)&data[offset + i * sizeof(float)] = value[i];
		}

		offset += sizeof(float) * 3;
	}

	template<typename T>
	void WriteVec4(const Vec4<T>& value)
	{
		if (sizeof(T) * 4 > size - offset)
		{
			N remainder = sizeof(T) * 4 - (size - offset);

			Byte* r = new Byte[size + remainder];

			for (N i = 0; i < size; ++i)
				r[i] = data[i];

			delete[] data;
			data = r;
			size += remainder;
		}

		if (CPU::GetEndianness() == Endianness::LE)
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < 4; ++i)
					*(T*)&data[offset + i * sizeof(T)] = value[i];
			else
				for (N i = 0; i < 4; ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = value[sizeof(T) - i - 1];
		}
		else
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < 4; ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = value[sizeof(T) - i - 1];
			else
				for (N i = 0; i < 4; ++i)
					*(T*)&data[offset + i * sizeof(T)] = value[i];
		}

		offset += sizeof(T) * 4;
	}

	template<typename T>
	void WriteRect(const Rect<T>& value)
	{
		if (sizeof(T) * 4 > size - offset)
		{
			N remainder = sizeof(T) * 4 - (size - offset);

			Byte* r = new Byte[size + remainder];

			for (N i = 0; i < size; ++i)
				r[i] = data[i];

			delete[] data;
			data = r;
			size += remainder;
		}

		if (CPU::GetEndianness() == Endianness::LE)
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < 4; ++i)
					*(T*)&data[offset + i * sizeof(T)] = value[i];
			else
				for (N i = 0; i < 4; ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = value[sizeof(T) - i - 1];
		}
		else
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < 4; ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = value[sizeof(T) - i - 1];
			else
				for (N i = 0; i < 4; ++i)
					*(T*)&data[offset + i * sizeof(T)] = value[i];
		}

		offset += sizeof(T) * 4;
	}

	void WriteColor4(const Color4& value)
	{
		if (sizeof(float) * 4 > size - offset)
		{
			N remainder = sizeof(float) * 4 - (size - offset);

			Byte* r = new Byte[size + remainder];

			for (N i = 0; i < size; ++i)
				r[i] = data[i];

			delete[] data;
			data = r;
			size += remainder;
		}

		if (CPU::GetEndianness() == Endianness::LE)
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < 4; ++i)
					*(float*)&data[offset + i * sizeof(float)] = value[i];
			else
				for (N i = 0; i < 4; ++i)
					for (N b = 0; b < sizeof(float); ++b)
						data[offset + i * sizeof(float) + b] = value[sizeof(float) - i - 1];
		}
		else
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < 4; ++i)
					for (N b = 0; b < sizeof(float); ++b)
						data[offset + i * sizeof(float) + b] = value[sizeof(float) - i - 1];
			else
				for (N i = 0; i < 4; ++i)
					*(float*)&data[offset + i * sizeof(float)] = value[i];
		}

		offset += sizeof(float) * 4;
	}

	template<typename T>
	void WriteQuat(const Quat<T>& value)
	{
		if (sizeof(T) * 4 > size - offset)
		{
			N remainder = sizeof(T) * 4 - (size - offset);

			Byte* r = new Byte[size + remainder];

			for (N i = 0; i < size; ++i)
				r[i] = data[i];

			delete[] data;
			data = r;
			size += remainder;
		}

		if (CPU::GetEndianness() == Endianness::LE)
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < 4; ++i)
					*(T*)&data[offset + i * sizeof(T)] = value[i];
			else
				for (N i = 0; i < 4; ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = value[sizeof(T) - i - 1];
		}
		else
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < 4; ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = value[sizeof(T) - i - 1];
			else
				for (N i = 0; i < 4; ++i)
					*(T*)&data[offset + i * sizeof(T)] = value[i];
		}

		offset += sizeof(T) * 4;
	}

	template<typename T>
	void WriteMat2(const Mat2<T>& value)
	{
		if (sizeof(T) * 4 > size - offset)
		{
			N remainder = sizeof(T) * 4 - (size - offset);

			Byte* r = new Byte[size + remainder];

			for (N i = 0; i < size; ++i)
				r[i] = data[i];

			delete[] data;
			data = r;
			size += remainder;
		}

		if (CPU::GetEndianness() == Endianness::LE)
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < 4; ++i)
						*(T*)&data[offset + i * sizeof(T)] = value[i];
			else
				for (N i = 0; i < 4; ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = ((Byte*)&value[i])[sizeof(T) - b - 1];
		}
		else
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < 4; ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = ((Byte*)&value[i])[sizeof(T) - b - 1];
			else
				for (N i = 0; i < 4; ++i)
					*(T*)&data[offset + i * sizeof(T)] = value[i];
		}

		offset += sizeof(T) * 4;
	}

	template<typename T>
	void WriteMat3(const Mat3<T>& value)
	{
		if (sizeof(T) * 9 > size - offset)
		{
			N remainder = sizeof(T) * 9 - (size - offset);

			Byte* r = new Byte[size + remainder];

			for (N i = 0; i < size; ++i)
				r[i] = data[i];

			delete[] data;
			data = r;
			size += remainder;
		}

		if (CPU::GetEndianness() == Endianness::LE)
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < 9; ++i)
					*(T*)&data[offset + i * sizeof(T)] = value[i];
			else
				for (N i = 0; i < 9; ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = ((Byte*)&value[i])[sizeof(T) - b - 1];
		}
		else
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < 9; ++i)
					for (N b = 0; b < sizeof(T); ++b)
						data[offset + i * sizeof(T) + b] = ((Byte*)&value[i])[sizeof(T) - b - 1];
			else
				for (N i = 0; i < 9; ++i)
					*(T*)&data[offset + i * sizeof(T)] = value[i];
		}

		offset += sizeof(T) * 9;
	}

	template<typename T>
	void WriteMat4(const Mat4<T>& value)
	{
		if (sizeof(T) * 16 > size - offset)
		{
			N remainder = sizeof(T) * 16 - (size - offset);

			Byte* r = new Byte[size + remainder];

			for (N i = 0; i < size; ++i)
				r[i] = data[i];

			delete[] data;
			data = r;
			size += remainder;
		}

		if (CPU::GetEndianness() == Endianness::LE)
		{
			if (endianness == Endianness::LE)
				for (N y = 0; y < 4; ++y)
					for (N x = 0; x < 4; ++x)
						*(T*)&data[offset + y * sizeof(T) * 4 + x * sizeof(T)] = value[y][x];
			else
				for (N y = 0; y < 4; ++y)
					for (N x = 0; x < 4; ++x)
						for (N i = 0; i < sizeof(T); ++i)
							data[offset + y * sizeof(T) * 4 + x * sizeof(T) + i] = ((Byte*)&value[y][x])[sizeof(T) - i - 1];
		}
		else
		{
			if (endianness == Endianness::LE)
				for (N y = 0; y < 4; ++y)
					for (N x = 0; x < 4; ++x)
						for (N i = 0; i < sizeof(T); ++i)
							data[offset + y * sizeof(T) * 4 + x * sizeof(T) + i] = ((Byte*)&value[y][x])[sizeof(T) - i - 1];
			else
				for (N y = 0; y < 4; ++y)
					for (N x = 0; x < 4; ++x)
						*(T*)&data[offset + y * sizeof(T) * 4 + x * sizeof(T)] = value[y][x];
		}

		offset += sizeof(T) * 16;
	}

	void WriteSer(const Serializer& ser)
	{
		if (ser.endianness != endianness)
			return;

		if (ser.Size() > size - offset)
		{
			N remainder = ser.Size() - (size - offset);

			Byte* result = new Byte[size + remainder];

			Util::Copy(result, data, size);

			delete[] data;
			data = result;
			size += remainder;
		}

		Util::Copy(&data[offset], &ser[0], ser.Size());

		offset += ser.Size();
	}

	template<typename T>
	void Write(const T value)
	{
		if (sizeof(T) > size - offset)
		{
			N remainder = sizeof(T) - (size - offset);

			Byte* r = new Byte[size + remainder];

			for (N i = 0; i < size; ++i)
				r[i] = data[i];

			delete[] data;
			data = r;
			size += remainder;
		}

		if (CPU::GetEndianness() == Endianness::LE)
		{
			if (endianness == Endianness::LE)
				*(T*)&data[offset] = value;
			else
				for (N i = 0; i < sizeof(T); ++i)
					data[offset + i] = ((Byte*)&value)[sizeof(T) - i - 1];
		}
		else
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < sizeof(T); ++i)
					data[offset + i] = ((Byte*)&value)[sizeof(T) - i - 1];
			else
				*(T*)&data[offset] = value;
		}

		offset += sizeof(T);
	}

	template<typename T, typename O>
	void ReadArray(T* const value, O* const size)
	{
		if (!*size)
			*size = (O)Read<N>();

		for (N i = 0; i < *size; ++i)
			value[i] = Read<T>();
	}

	template<typename T, typename O>
	Array<T, O> ReadArray(const O size = 0)
	{
		Array<T, O> result(size ? size : (O)Read<N>());

		for (O i = 0; i < result.Size(); ++i)
			result[i] = Read<T>();

		return result;
	}

	template<typename T, typename O>
	Vector<T, O> ReadVector(const O size = 0)
	{
		Vector<T, O> result(size ? size : (O)Read<N>());

		for (O i = 0; i < result.Size(); ++i)
			result[i] = Read<T>();

		return result;
	}

	template<typename T, typename O>
	Str<T, O> ReadStr(const O size = 0)
	{
		Str<T, O> result(size ? size : (O)Read<N>());

		for (O i = 0; i < result.Size(); ++i)
			result[i] = Read<T>();

		return result;
	}

	Version ReadVersion()
	{
		return {Read<unsigned int>(), Read<unsigned int>(), Read<unsigned int>()};
	}

	template<typename T>
	Vec2<T> ReadVec2()
	{
		return {Read<T>(), Read<T>()};
	}

	template<typename T>
	Vec3<T> ReadVec3()
	{
		return {Read<T>(), Read<T>(), Read<T>()};
	}

	Color3 ReadColor3()
	{
		return {Read<float>(), Read<float>(), Read<float>()};
	}

	template<typename T>
	Vec4<T> ReadVec4()
	{
		return {Read<T>(), Read<T>(), Read<T>(), Read<T>()};
	}

	template<typename T>
	Rect<T> ReadRect()
	{
		return {Read<T>(), Read<T>(), Read<T>(), Read<T>()};
	}

	Color4 ReadColor4()
	{
		return {Read<float>(), Read<float>(), Read<float>(), Read<float>()};
	}

	template<typename T>
	Quat<T> ReadQuat()
	{
		return {Read<T>(), Read<T>(), Read<T>(), Read<T>()};
	}

	template<typename T>
	Mat2<T> ReadMat2()
	{
		Mat2<T> result;
		for (N i = 0; i < 4; ++i)
			result[i] = Read<float>();

		return result;
	}

	template<typename T>
	Mat3<T> ReadMat3()
	{
		Mat3<T> result;
		for (N i = 0; i < 9; ++i)
			result[i] = Read<float>();

		return result;
	}

	template<typename T>
	Mat4<T> ReadMat4()
	{
		Mat4<T> result;
		for (N i = 0; i < 16; ++i)
			result[i] = Read<float>();

		return result;
	}

	template<typename T>
	T Read()
	{
		T value = (T)0;

		if (CPU::GetEndianness() == Endianness::LE)
		{
			if (endianness == Endianness::LE)
				value = *(T*)&data[offset];
			else
				for (N i = 0; i < sizeof(T); ++i)
					((Byte*)&value)[sizeof(T) - i - 1] = data[offset + i];
		}
		else
		{
			if (endianness == Endianness::LE)
				for (N i = 0; i < sizeof(T); ++i)
					((Byte*)&value)[sizeof(T) - i - 1] = data[offset + i];
			else
				value = *(T*)&data[offset];
		}

		offset += sizeof(T);

		return value;
	}

	void SetOffset(const N offset)
	{
		this->offset = offset;
	}

	N GetOffset() const
	{
		return offset;
	}

	void Resize(const N newSize)
	{
		Byte* result = new Byte[newSize];
		Util::Copy(result, data, size);
		delete[] data;
		data = result;
		size = newSize;
	}

	N Size() const
	{
		return size;
	}
};