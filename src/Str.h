#pragma once

#include "EHS.h"
#include "Util.h"
#include "Vector.h"

#include <utility>

enum class SearchPattern
{
	LEFT_RIGHT,
	RIGHT_LEFT
};

enum class IndexResult
{
	BEGINNING,
	ENDING
};

/// A helper class for C-style strings.
/// @tparam T The character's data type to use.
/// @tparam N The number data type to use.
template<typename T = Char_8, typename N = UInt_64>
class Str
{
private:
	N size;
	T* data;

public:
	/// Frees any data created on the heap.
	~Str()
	{
		delete[] data;
	}

	/// Default members initialization.
	Str()
		: size(0), data(nullptr)
	{
	}

	/// Initializes members with given C-style string.
	/// @param [in] str The C-style string.
	/// @param [in] size The size of the given C-style string.
	Str(const T* const str, const N size)
		: size((size) ? size : Len(str)), data(nullptr)
	{
		data = new T[this->size + 1];

		Util::Copy(data, str, Size(true));

		data[this->size] = 0;
	}

	/// Initializes members with given C-style string.
	/// @param [in] str The C-style string.
	Str(const T* const str)
		: size(Len(str)), data(nullptr)
	{
		data = new T[size + 1];

		Util::Copy(data, str, Size(true));

		data[size] = 0;
	}

	/// Initializes string with the given size.
	/// @param [in] size The size.
	Str(const N size)
		: size(size), data(new T[size + 1])
	{
		data[size] = 0;
	}

	/// Copies all members from the given string object.
	/// @param [in] str The string object to copy from.
	Str(const Str& str)
		: size(str.size), data(new T[size + 1])
	{
		Util::Copy(data, str.data, Size(true));

		data[size] = 0;
	}

	Str(Str&& str) noexcept
		: size(str.size), data(str.data)
	{
		str.size = 0;
		str.data = nullptr;
	}

	/// Copies all members from the given string object.
	/// @param [in] str The string object to copy from.
	/// @returns The string that has been assigned to.
	Str<T, N>& operator=(const Str<T, N>& str)
	{
		if (&str == this)
			return *this;

		size = str.size;

		delete[] data;
		data = new T[size + 1];
		Util::Copy(data, str.data, Size(true));
		data[size] = 0;

		return *this;
	}

	Str& operator=(Str&& str) noexcept
	{
		if (this == &str)
			return *this;

		size = str.size;
		delete[] data;
		data = str.data;

		str.size = 0;
		str.data = nullptr;

		return *this;
	}

	/// Copies the given C-style string and retrieves the size.
	/// @param [in] str The C-style string to copy from.
	/// @returns The string object that has been assigned to.
	Str<T, N>& operator=(const T* const str)
	{
		size = Len(str);

		delete[] data;

		data = new T[size + 1];

		Util::Copy(data, str, Size(true));

		data[size] = 0;

		return *this;
	}

	/// Concatenates with the given C-style string.
	/// @param [in] str The given C-style string.
	/// @returns The result.
	Str<T, N>& operator+=(const T* const str)
	{
		N inputSize = Len(str);

		T* result = new T[size + inputSize + 1];

		Util::Copy(result, data, Size(true));

		delete[] data;

		Util::Copy(&result[size], str, inputSize * sizeof(T));

		result[size + inputSize] = 0;
		size += inputSize;
		data = result;

		return *this;
	}

	/// Concatenates with the given string object.
	/// @param [in] str The given string object.
	/// @returns The result.
	Str<T, N>& operator+=(const Str<T, N>& str)
	{
		T* result = new T[size + str.size + 1];

		Util::Copy(result, data, Size(true));

		delete[] data;

		Util::Copy(&result[size], str, str.Size(true));

		result[size + str.size] = 0;
		size += str.Size();
		data = result;

		return *this;
	}

	/// Concatenates with the given number.
	/// @param [in] num The given number to concatenate.
	/// @returns The result.
	Str<T, N>& operator+=(const SInt_64 num)
	{
		return operator+=(FromNum(num));
	}

	/// Concatenates with the given number.
	/// @param [in] num The given number to concatenate.
	/// @returns The result.
	Str<T, N>& operator+=(const UInt_64 num)
	{
		return operator+=(FromNum(num));
	}

	/// Concatenates with the given number.
	/// @param [in] num The given number to concatenate.
	/// @returns The result.
	Str<T, N>& operator+=(const SInt_32 num)
	{
		return operator+=(FromNum(num));
	}

	/// Concatenates with the given number.
	/// @param [in] num The given number to concatenate.
	/// @returns The result.
	Str<T, N>& operator+=(const UInt_32 num)
	{
		return operator+=(FromNum(num));
	}

	/// Concatenates with the given number.
	/// @param [in] num The given number to concatenate.
	/// @returns The result.
	Str<T, N>& operator+=(const SInt_16 num)
	{
		return operator+=(FromNum(num));
	}

	/// Concatenates with the given number.
	/// @param [in] num The given number to concatenate.
	/// @returns The result.
	Str<T, N>& operator+=(const UInt_16 num)
	{
		return operator+=(FromNum(num));
	}

	/// Concatenates with the given number.
	/// @param [in] num The given number to concatenate.
	/// @returns The result.
	Str<T, N>& operator+=(const SInt_8 num)
	{
		return operator+=(FromNum(num));
	}

	/// Concatenates with the given number.
	/// @param [in] num The given number to concatenate.
	/// @returns The result.
	Str<T, N>& operator+=(const UInt_8 num)
	{
		return operator+=(FromNum(num));
	}

	#ifdef OS_WINDOWS
		/// Concatenates with the given number.
		/// @param [in] num The given number to concatenate.
		/// @returns The result.
		Str<T, N>& operator+=(const DWORD num)
		{
			return operator+=(FromNum(num));
		}

		/// Concatenates with the given number.
		/// @param [in] num The given number to concatenate.
		/// @returns The result.
		Str<T, N>& operator+=(const HRESULT num)
		{
			return operator+=(FromNum(num));
		}
	#endif

	/// Concatenates with the given number.
	/// @param [in] num The given number to concatenate.
	/// @returns The result.
	Str<T, N>& operator+=(const float num)
	{
		return operator+=(FromNum(num));
	}

	/// Concatenates with the given number.
	/// @param [in] num The given number to concatenate.
	/// @returns The result.
	Str<T, N>& operator+=(const double num)
	{
		return operator+=(FromNum(num));
	}

	/// Concatenates with the given number.
	/// @param [in] num The given number to concatenate.
	/// @returns The result.
	Str<T, N>& operator+=(const long double num)
	{
		return operator+=(FromNum(num));
	}

	/// Concatenates with the given C-style string.
	/// @param [in] str The given C-style string.
	/// @returns The result.
	Str<T, N> operator+(const T* const str) const
	{
		N inSize = Len(str);

		Str<T, N> result(size + inSize);

		Util::Copy(result.data, data, Size(true));

		Util::Copy(&result.data[size], str, inSize * sizeof(T));

		result.data[size + inSize] = 0;

		return result;
	}

	/// Concatenates with the given string object.
	/// @param [in] str The given string object.
	/// @returns The result.
	Str<T, N> operator+(const Str<T, N>& str) const
	{
		Str<T, N> result(size + str.size);

		Util::Copy(result.data, data, Size(true));

		Util::Copy(&result.data[size], str.data, str.Size(true));

		result.data[size + str.size] = 0;

		return result;
	}

	/// Concatenates with the given number.
	/// @param [in] str The given number to Concatenate.
	/// @returns The result.
	Str<T, N> operator+(const SInt_64 num) const
	{
		return operator+(FromNum(num));
	}

	/// Concatenates with the given number.
	/// @param [in] str The given number to Concatenate.
	/// @returns The result.
	Str<T, N> operator+(const UInt_64 num) const
	{
		return operator+(FromNum(num));
	}

	/// Concatenates with the given number.
	/// @param [in] str The given number to Concatenate.
	/// @returns The result.
	Str<T, N> operator+(const SInt_32 num) const
	{
		return operator+(FromNum(num));
	}

	/// Concatenates with the given number.
	/// @param [in] str The given number to Concatenate.
	/// @returns The result.
	Str<T, N> operator+(const UInt_32 num) const
	{
		return operator+(FromNum(num));
	}

	/// Concatenates with the given number.
	/// @param [in] str The given number to Concatenate.
	/// @returns The result.
	Str<T, N> operator+(const SInt_16 num) const
	{
		return operator+(FromNum(num));
	}

	/// Concatenates with the given number.
	/// @param [in] str The given number to Concatenate.
	/// @returns The result.
	Str<T, N> operator+(const UInt_16 num) const
	{
		return operator+(FromNum(num));
	}

	/// Concatenates with the given number.
	/// @param [in] str The given number to Concatenate.
	/// @returns The result.
	Str<T, N> operator+(const SInt_8 num) const
	{
		return operator+(FromNum(num));
	}

	/// Concatenates with the given number.
	/// @param [in] str The given number to Concatenate.
	/// @returns The result.
	Str<T, N> operator+(const UInt_8 num) const
	{
		return operator+(FromNum(num));
	}

	#ifdef OS_WINDOWS
		/// Concatenates with the given number.
		/// @param [in] str The given number to Concatenate.
		/// @returns The result.
		Str<T, N> operator+(const DWORD num) const
		{
			return operator+(FromNum(num));
		}

		/// Concatenates with the given number.
		/// @param [in] str The given number to Concatenate.
		/// @returns The result.
		Str<T, N> operator+(const HRESULT num) const
		{
			return operator+(FromNum(num));
		}
	#endif

	/// Concatenates with the given number.
	/// @param [in] str The given number to Concatenate.
	/// @returns The result.
	Str<T, N> operator+(const float num) const
	{
		return operator+(FromNum(num));
	}

	/// Concatenates with the given number.
	/// @param [in] str The given number to Concatenate.
	/// @returns The result.
	Str<T, N> operator+(const double num) const
	{
		return operator+(FromNum(num));
	}

	/// Concatenates with the given number.
	/// @param [in] str The given number to Concatenate.
	/// @returns The result.
	Str<T, N> operator+(const long double num) const
	{
		return operator+(FromNum(num));
	}

	bool operator==(T* str) const
	{
		if (size != Len(str))
			return false;

		return Util::IsEqual(data, str, Size(true));
	}

	/// Compares with a C-style string. First comparing sizes.
	/// @param [in] str The C-style string to compare with.
	/// @returns Whether or not they are equal.
	bool operator==(const T* const str) const
	{
		if (size != Len(str))
			return false;

		return Util::IsEqual(data, str, Size(true));
	}

	/// Compares with a string object. First comparing sizes.
	/// @param [in] str The string object to compare with.
	/// @returns Whether or not they are equal.
	bool operator==(const Str<T, N>& str) const
	{
		if (size != str.size)
			return false;

		return Util::IsEqual(data, str, Size(true));
	}

	/// Compares with a C-style string. First comparing sizes.
	/// @param [in] str The C-style string to compare with.
	/// @returns Whether or not they are equal.
	bool operator!=(const T* const str) const
	{
		if (size != Len(str))
			return true;

		return !Util::IsEqual(data, str, Size(true));
	}

	/// Compares with a string object. First comparing sizes.
	/// @param [in] str The string to compare with.
	/// @returns Whether or not they are equal.
	bool operator!=(const Str<T, N>& str) const
	{
		if (size != str.size)
			return true;

		return !Util::IsEqual(data, str, Size(true));
	}

	/// Retrieves the raw C-style string from casting a string object.
	operator T* () const
	{
		return data;
	}

	/// Resizes the string.
	/// @param [in] newSize The size to change to.
	void Resize(const N newSize)
	{
		if (newSize == size)
			return;

		T* result = new T[newSize + 1];

		if (newSize > size)
			Util::Copy(result, data, Size(true));
		else
			Util::Copy(result, data, newSize * sizeof(T));

		size = newSize;

		delete[] data;

		data = result;
		data[size] = 0;
	}

	/// Retrieves the size of the string.
	/// @param [in] inBytes To retrieve the size in bytes.
	/// @returns The resulting size.
	N Size(bool inBytes = false) const
	{
		if (inBytes)
			return sizeof(T) * size;
		else
			return size;
	}

	/// Finds a null terminator in the string and makes it the exact size if greater than.
	void ExactSize()
	{
		size = Len(data);

		T* result = new T[size + 1];
		Util::Copy(result, data, Size(true));

		delete[] data;

		result[size] = 0;
		data = result;
	}

	/// Copies a string object to the referenced string object.
	/// @param dstOffset The offset index to copy to.
	/// @param src The source string object to copy from.
	void Copy(const N dstOffset, const Str<T, N>& src)
	{
		if (dstOffset + src.size > size)
		{
			//Log_8 log(__FUNCSIG__, LogType::ERR, 0, "The parameters \"dstOffset\" (" + Str_8::FromNum(dstOffset) + ") + \"srcSize\" (" + Str_8::FromNum(src.Size()) + ") is greater than the referenced string object's size (" + Str_8::FromNum(size) + ").");
			return;
		}

		Util::Copy(data[dstOffset], src, src.Size(true));
	}

	/// Copies a C-style string to the referenced string object.
	/// @param dstOffset The offset index to copy to.
	/// @param src The source C-style string to copy from.
	void Copy(const N dstOffset, const T* const src)
	{
		N srcSize = Len(src);

		if (dstOffset + srcSize > size)
		{
			//Log_8 log(__FUNCSIG__, LogType::ERR, 0, "The parameters \"dstOffset\" (" + Str_8::FromNum(dstOffset) + ") + \"srcSize\" (" + Str_8::FromNum(srcSize) + ") is greater than the referenced string object's size (" + Str_8::FromNum(size) + ").");
			return;
		}

		Util::Copy(data[dstOffset], src, srcSize * sizeof(T));
	}

	/// Copies a C-style string to the referenced string object.
	/// @param dstOffset The offset index to copy to.
	/// @param src The source C-style string to copy from.
	/// @param srcSize The size of the source C-style string.
	void Copy(const N dstOffset, const T* const src, const N srcSize)
	{
		if (dstOffset + srcSize > size)
		{
			//Log_8 log(__FUNCSIG__, LogType::ERR, 0, "The parameters \"dstOffset\" (" + Str_8::FromNum(dstOffset) + ") + \"srcSize\" (" + Str_8::FromNum(srcSize) + ") is greater than the referenced string object's size (" + Str_8::FromNum(size) + ").");
			return;
		}

		Util::Copy(data[dstOffset], src, srcSize * sizeof(T));
	}

	/// Adds a value at the end of the string.
	/// @param [in] value The character to push to the end of the string.
	/// @note Automatically moves the null terminator after the value is pushed.
	void Push(const Str<T, N> &value)
	{
		T* result = new T[size + value.size + 1];
		Util::Copy(result, data, Size(true));
		Util::Copy(&result[size], value, value.Size(true));

		result[size + value.Size()] = 0;

		delete[] data;

		data = result;
		size += value.size;
	}

	/// Adds a value at the end of the string.
	/// @param [in] value The C-style string to push to the end of the string.
	/// @param [in] size The size of the given C-style string.
	/// @note Automatically moves the null terminator after the value is pushed.
	void Push(const T* const value, const N size)
	{
		T* result = new T[this->size + size + 1];
		Util::Copy(result, data, Size(true));
		Util::Copy(&result[this->size], value, size * sizeof(T));

		result[this->size + size] = 0;

		delete[] data;

		data = result;
		this->size += size;
	}

	/// Adds a value at the end of the string.
	/// @param [in] value The C-style string to push to the end of the string.
	/// @note Automatically moves the null terminator after the value is pushed.
	void Push(const T* const value)
	{
		N inSize = Len(value);

		T* result = new T[size + inSize + 1];
		Util::Copy(result, data, Size(true));
		Util::Copy(result[size], value, inSize * sizeof(T));

		result[size + inSize] = 0;

		delete[] data;

		data = result;
		size += inSize;
	}

	/// Adds a value at the end of the string.
	/// @param [in] value The character to push to the end of the string.
	/// @note Automatically moves the null terminator after the value is pushed.
	void Push(const T value)
	{
		T* result = new T[size + 2];

		Util::Copy(result, data, Size(true));

		result[size] = value;
		result[size + 1] = 0;

		delete[] data;

		data = result;
		++size;
	}

	/// Removes a value at the end of the array.
	/// @returns The value that was popped.
	T Pop()
	{
		T* result = new T[size--];

		T value = data[size];

		Util::Copy(result, data, Size(true));

		result[size] = 0;

		delete[] data;

		data = result;

		return value;
	}

	/// Retrieves the string in bytes for serialization.
	/// @returns The string in bytes.
	const Byte* ToBytes() const
	{
		return (Byte*)data;
	}

	/// Retrieves the string in bytes for serialization.
	/// @returns The string in bytes.
	Byte* ToBytes()
	{
		return (Byte*)data;
	}

	/// Changes all upper-case ASCII characters to lower-case.
	void ToLower()
	{
		for (N i = 0; i < size; ++i)
			if (data[i] > 64 && data[i] < 91)
				data[i] += 32;
	}

	/// Changes all upper-case ASCII characters to lower-case.
	/// @returns The result.
	Str<T, N> GetLower() const
	{
		Str<T, N> result(size);

		for (N i = 0; i < size; ++i)
			if (data[i] > 64 && data[i] < 91)
				result[i] = data[i] + 32;
			else
				result[i] = data[i];

		return result;
	}

	/// Changes all lower-case ASCII characters to upper-case.
	void ToUpper()
	{
		for (N i = 0; i < size; ++i)
			if (data[i] > 96 && data[i] < 123)
				data[i] -= 32;
	}

	/// Changes all lower-case ASCII characters to upper-case.
	/// @returns The result.
	Str<T, N> GetUpper() const
	{
		Str<T, N> result(size);

		for (N i = 0; i < size; ++i)
			if (data[i] > 96 && data[i] < 123)
				result[i] = data[i] - 32;
			else
				result[i] = data[i];

		return result;
	}

	/// Reverses the entire referenced string object.
	void Reverse()
	{
		if (size <= 1 || !data)
			return;

		T* r = new T[size + 1];

		for (N i = 0; i < size; ++i)
			r[i] = data[size - 1 - i];

		r[size] = 0;

		delete[] data;

		data = r;
	}

	/// Reverses the entire string object.
	/// @returns The result.
	Str<T, N> GetReverse()
	{
		if (size <= 1 || !data)
			return *this;

		Str<T, N> r(size);

		for (N i = 0; i < size; ++i)
			r[i] = data[size - 1 - i];

		return r;
	}

	/// Clips the string at the given index and with the given size.
	/// @param [in] index The index to clip at.
	/// @param [in] size The size for the clip starting from the index.
	/// @returns The result.
	Str<T, N> Sub(const N index, const N size = 0) const
	{
		if (index >= this->size)
		{
			//Log_8 log(__FUNCSIG__, LogType::ERR, 0, "The given index parameter, \"" + Str<T, N>::FromNum(index) + "\" is greater than the string's size, \"" + Str<T, N>::FromNum(this->size) + "\".");

			return Str<T, N>();
		}

		if (size)
		{
			if (size > this->size)
			{
				//Log_8 log(__FUNCSIG__, LogType::ERR, 1, "The given size parameter, \"" + Str<T, N>::FromNum(size) + "\" is greater than the string's size, \"" + Str<T, N>::FromNum(this->size) + "\", at the index, \"" + Str<T, N>::FromNum(index) + "\".");

				return Str<T, N>();
			}

			Str<T, N> result(size);

			Util::Copy(result, &data[index], result.Size(true));

			return result;
		}
		else
		{
			Str<T, N> result(this->size - index);

			Util::Copy(result, &data[index], result.Size(true));

			return result;
		}
	}

	/// Splits a string into an array with the given separator.
	/// @param [in] ide The given string as the separator.
	/// @param [in] max The max amount of times to split the string.
	/// @returns The result.
	Vector<Str<T, N>, N> Split(const Str<T, N>& ide, const N max = 0) const
	{
		Vector<Str<T, N>, N> result(0, 5);

		N b = 0;

		for (N i = 0, c = 0; i < size; ++i)
		{
			if (data[i] == ide[c])
			{
				if (++c == ide.Size())
				{
					N r = i - (c - 1) - b;
					if (!r)
						result.Push(Str<T, N>());
					else
						result.Push(Str<T, N>(&data[b], r));

					b = i + 1;
					c = 0;

					if (max && result.Size() == max)
						break;
				}
			}
			else
			{
				c = 0;
			}
		}

		if (b < size)
			result.Push(Str<T, N>(&data[b], size - b));

		return result;
	}

	/// Removes all instances of the given string object.
	/// @param [in] ide The string to look for.
	/// @returns The result.
	Str<T, N> RemoveAll(const Str<T, N>& ide) const
	{
		Str<T, N> result(size);

		for (N i = 0, b = 0, c = 0; i < size; ++i)
		{
			if (data[i] == ide[c])
			{
				if (++c == ide.Size())
					c = 0;
			}
			else
			{
				if (c)
					for (N d = c; d < 0; --d)
						result[b++] = data[i - d];
				else
					result[b++] = data[i];

				c = 0;
			}
		}

		return result;
	}

	Str ReplaceAll(const Str& ide, const Str& replacer) const
	{
		Str<T, N> result;

		for (N i = 0, b = 0; i < size; ++i)
		{
			if (data[i] == ide[b])
			{
				if (++b == ide.Size())
				{
					result.Push(replacer);
					b = 0;
				}
			}
			else
			{
				result.Push(data[i]);
			}
		}

		return result;
	}

	/// Finds the first instance of the given string object.
	/// @param [in] ide The string to look for.
	/// @param [out] index The index of the string found. Can be a nullptr.
	/// @param [in] pattern The search pattern for optimization.
	/// @param [in] result What index to return where the first instance is found.
	/// @returns The index where the instance was found with the result varying from the result parameter.
	bool Find(const Str<T, N>& ide, N* const index = nullptr, const SearchPattern pattern = SearchPattern::LEFT_RIGHT, const IndexResult result = IndexResult::BEGINNING) const
	{
		if (pattern == SearchPattern::LEFT_RIGHT)
		{
			for (N i = 0, c = 0; i < size; ++i)
			{
				if (data[i] == ide[c])
				{
					if (++c == ide.Size())
					{
						if (result == IndexResult::BEGINNING)
						{
							if (index)
								*index = i - (c - 1);

							return true;
						}
						else
						{
							if (index)
								*index = i;

							return true;
						}
					}
				}
			}
		}
		else if (pattern == SearchPattern::RIGHT_LEFT)
		{
			for (N i = size, c = ide.Size(); i > 0; --i)
			{
				if (data[i - 1] == ide[c - 1])
				{
					if (--c == 0)
					{
						if (result == IndexResult::BEGINNING)
						{
							if (index)
								*index = i - (ide.Size() - 1);

							return true;
						}
						else
						{
							if (index)
								*index = i;

							return true;
						}
					}
				}
			}
		}

		return false;
	}

	bool Contains(const Str<T, N>& ide, const SearchPattern pattern = SearchPattern::LEFT_RIGHT) const
	{
		if (pattern == SearchPattern::LEFT_RIGHT)
		{
			for (N i = 0, c = 0; i < size; ++i)
			{
				if (data[i] == ide[c])
				{
					if (++c == ide.Size())
					{
						return true;
					}
				}
			}
		}
		else if (pattern == SearchPattern::RIGHT_LEFT)
		{
			for (N i = size, c = ide.Size(); i > 0; --i)
			{
				if (data[i - 1] == ide[c - 1])
				{
					if (--c == 0)
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	/// Checks if the string represents a number. Must not have any alphabetical characters.
	/// @returns The result.
	bool IsNum() const
	{
		if (!size)
			return false;

		if ((data[0] < '0' || data[0] > '9') && data[0] != '-' && data[0] != '.')
			return false;

		for (N i = 1; i < size; ++i)
			if ((data[i] < '0' || data[i] > '9') && data[i] != '.')
				return false;

		return true;
	}

	template<typename I = int>
	static Str NumToHex(const I num)
	{
		static const T hex[] = "0123456789ABCDEF";

		Str result(sizeof(I) * 2);

		for (UInt_8 i = 0; i < sizeof(I); ++i)
		{
			result[i * 2] = hex[((Byte*)&num)[i] / 16];
			result[i * 2 + 1] = hex[((Byte*)&num)[i] % 16];
		}

		return result;
	}

	template<typename I = int>
	static I HexToNum(const Str& in)
	{
		N offset = 0;
		bool neg = false;

		if (in[offset] == 45)
		{
			neg = true;
			++offset;
		}
		else if (in[offset] == 43)
			++offset;

		if (in[offset] == 48 && (in[offset + 1] == 88 || in[offset + 1] == 120))
			offset += 2;

		N acc = 0;

		for (N i = offset; i < in.Size(); ++i)
		{
			I value = 0;

			if (in[i] > 47 && in[i] < 58)
				value = in[i] - 48;
			else if (in[i] > 64 && in[i] < 71)
				value = in[i] - 55;
			else if (in[i] > 96 && in[i] < 103)
				value = in[i] - 87;

			if (value >= 16)
				return 0;

			acc *= 16;
			acc += value;
		}

		return neg ? -acc : acc;
	}

	template<typename I = int>
	I HexToNum() const
	{
		N offset = 0;
		bool neg = false;

		if (data[offset] == 45)
		{
			neg = true;
			++offset;
		}
		else if (data[offset] == 43)
			++offset;

		if (data[offset] == 48 && (data[offset + 1] == 88 || data[offset + 1] == 120))
			offset += 2;

		N acc = 0;

		for (N i = offset; i < size; ++i)
		{
			I value = 0;

			if (data[i] > 47 && data[i] < 58)
				value = data[i] - 48;
			else if (data[i] > 64 && data[i] < 71)
				value = data[i] - 55;
			else if (data[i] > 96 && data[i] < 103)
				value = data[i] - 87;

			if (value >= 16)
				return 0;

			acc *= 16;
			acc += value;
		}

		return neg ? -acc : acc;
	}

	/// Converts the string into a number.
	/// @tparam I The resulting number's data type.
	/// @returns The result.
	/// @note Use "IsNum" before this if the referenced string object will not always be a number.
	template<typename I = N>
	I ToDecimal() const
	{
		I r = 0;

		if (!size)
			return r;

		for (N i = data[0] == '-' ? 1 : 0; i < size; ++i)
			r = r * 10 + data[i] - '0';

		if (data[0] == '-')
			r *= -1;

		return r;
	}

	float ToFloat() const
	{
		N decPoint = size;
		Find(".", &decPoint);

		float result = 0.0f;
		float fraction = 0.0f;
		float scale = 1.0f;

		for (N i = 0; i < decPoint; ++i)
			result = result * 10.0f + data[i] - '0';

		for (N i = decPoint + 1; i < size; ++i)
		{
			fraction = fraction * 10.0f + data[i] - '0';
			scale *= 10.0f;
		}

		result += fraction / scale;

		return result;
	}

	double ToDouble() const
	{
		N decPoint = size;
		Find(".", &decPoint);

		double result = 0.0f;
		double fraction = 0.0f;
		double scale = 1.0f;

		for (N i = 0; i < decPoint; ++i)
			result = result * 10.0f + data[i] - '0';

		for (N i = decPoint + 1; i < size; ++i)
		{
			fraction = fraction * 10.0f + data[i] - '0';
			scale *= 10.0f;
		}

		result += fraction / scale;

		return result;
	}

	long double ToLDouble() const
	{
		N decPoint = size;
		Find(".", &decPoint);

		long double result = 0.0f;
		long double fraction = 0.0f;
		long double scale = 1.0f;

		for (N i = 0; i < decPoint; ++i)
			result = result * 10.0f + data[i] - '0';

		for (N i = decPoint + 1; i < size; ++i)
		{
			fraction = fraction * 10.0f + data[i] - '0';
			scale *= 10.0f;
		}

		result += fraction / scale;

		return result;
	}

	/// Converts the given number into a string.
	/// @returns The result.
	static Str<T, N> FromNum(const SInt_64 num)
	{
		if (num == 0)
		{
			Str<T, N> z(1);
			z[0] = 48;
			return z;
		}

		Str<T, N> r(21);

		SInt_64 nonNeg;
		if (num < 0)
			nonNeg = -num;
		else
			nonNeg = num;

		N i = 0;

		while (nonNeg != 0)
		{
			r[i++] = 48 + nonNeg % 10;
			nonNeg /= 10;
		}

		if (num < 0)
			r[i++] = 45;

		r.Resize(i);

		return r.GetReverse();
	}

	/// Converts the given number into a string.
	/// @returns The result.
	static Str<T, N> FromNum(const UInt_64 num)
	{
		if (num == 0)
		{
			Str<T, N> z(1);
			z[0] = 48;
			return z;
		}

		Str<T, N> r(21);

		UInt_64 nonNeg = num;

		N i = 0;

		while (nonNeg != 0)
		{
			r[i++] = 48 + nonNeg % 10;
			nonNeg /= 10;
		}

		r.Resize(i);

		return r.GetReverse();
	}

	/// Converts the given number into a string.
	/// @returns The result.
	static Str<T, N> FromNum(const SInt_32 num)
	{
		if (num == 0)
		{
			Str<T, N> z(1);
			z[0] = 48;
			return z;
		}

		Str<T, N> r(11);

		SInt_32 nonNeg;
		if (num < 0)
			nonNeg = -num;
		else
			nonNeg = num;

		N i = 0;

		while (nonNeg != 0)
		{
			r[i++] = 48 + nonNeg % 10;
			nonNeg /= 10;
		}

		if (num < 0)
			r[i++] = 45;

		r.Resize(i);

		return r.GetReverse();
	}

	/// Converts the given number into a string.
	/// @returns The result.
	static Str<T, N> FromNum(const UInt_32 num)
	{
		if (num == 0)
		{
			Str<T, N> z(1);
			z[0] = 48;
			return z;
		}

		Str<T, N> r(11);

		UInt_32 nonNeg = num;

		N i = 0;

		while (nonNeg != 0)
		{
			r[i++] = 48 + nonNeg % 10;
			nonNeg /= 10;
		}

		r.Resize(i);

		return r.GetReverse();
	}

	/// Converts the given number into a string.
	/// @returns The result.
	static Str<T, N> FromNum(const SInt_16 num)
	{
		if (num == 0)
		{
			Str<T, N> z(1);
			z[0] = 48;
			return z;
		}

		Str<T, N> r(6);

		SInt_16 nonNeg;
		if (num < 0)
			nonNeg = -num;
		else
			nonNeg = num;

		N i = 0;

		while (nonNeg != 0)
		{
			r[i++] = 48 + nonNeg % 10;
			nonNeg /= 10;
		}

		if (num < 0)
			r[i++] = 45;

		r.Resize(i);

		return r.GetReverse();
	}

	/// Converts the given number into a string.
	/// @returns The result.
	static Str<T, N> FromNum(const UInt_16 num)
	{
		if (num == 0)
		{
			Str<T, N> z(1);
			z[0] = 48;
			return z;
		}

		Str<T, N> r(6);

		UInt_16 nonNeg = num;

		N i = 0;

		while (nonNeg != 0)
		{
			r[i++] = 48 + nonNeg % 10;
			nonNeg /= 10;
		}

		r.Resize(i);

		return r.GetReverse();
	}

	/// Converts the given number into a string.
	/// @returns The result.
	static Str<T, N> FromNum(const SInt_8 num)
	{
		if (num == 0)
		{
			Str<T, N> z(1);
			z[0] = 48;
			return z;
		}

		Str<T, N> r(4);

		SInt_8 nonNeg;
		if (num < 0)
			nonNeg = -num;
		else
			nonNeg = num;

		N i = 0;

		while (nonNeg != 0)
		{
			r[i++] = 48 + nonNeg % 10;
			nonNeg /= 10;
		}

		if (num < 0)
			r[i++] = 45;

		r.Resize(i);

		return r.GetReverse();
	}

	/// Converts the given number into a string.
	/// @returns The result.
	static Str<T, N> FromNum(const UInt_8 num)
	{
		if (num == 0)
		{
			Str<T, N> z(1);
			z[0] = 48;
			return z;
		}

		Str<T, N> r(4);

		UInt_8 nonNeg = num;

		N i = 0;

		while (nonNeg != 0)
		{
			r[i++] = 48 + nonNeg % 10;
			nonNeg /= 10;
		}

		r.Resize(i);

		return r.GetReverse();
	}

	#ifdef OS_WINDOWS
		/// Converts the given number into a string.
		/// @returns The result.
		static Str<T, N> FromNum(const DWORD num)
		{
			if (num == 0)
			{
				Str<T, N> z(1);
				z[0] = 48;
				return z;
			}

			Str<T, N> r(11);

			DWORD nonNeg = num;

			N i = 0;

			while (nonNeg != 0)
			{
				r[i++] = 48 + nonNeg % 10;
				nonNeg /= 10;
			}

			r.Resize(i);

			return r.GetReverse();
		}

		/// Converts the given number into a string.
		/// @returns The result.
		static Str<T, N> FromNum(const HRESULT num)
		{
			if (num == 0)
			{
				Str<T, N> z(1);
				z[0] = 48;
				return z;
			}

			Str<T, N> r(11);

			HRESULT nonNeg;
			if (num < 0)
				nonNeg = -num;
			else
				nonNeg = num;

			N i = 0;

			while (nonNeg != 0)
			{
				r[i++] = 48 + nonNeg % 10;
				nonNeg /= 10;
			}

			if (num < 0)
				r[i++] = 45;

			r.Resize(i);

			return r.GetReverse();
		}
	#endif

	/// Converts the given float into a string.
	/// @returns The result.
	static Str<T, N> FromNum(const float num, const UInt_8 maxDecimals = 5)
	{
		SInt_64 whole = (SInt_64)num;

		Str<T, N> result;
		if (whole < 0)
			result += "-";

		result += Str<T, N>::FromNum(whole);

		UInt_64 power = 10;
		for (UInt_64 i = 0; i < (UInt_64)maxDecimals - 1; ++i)
			power *= 10;

		SInt_64 fraction = (SInt_64)((num - (float)whole) * (float)power);
		if (!fraction)
			return result;

		result += ".";

		Str<T, N> fResult(maxDecimals);
		N i = 0;

		while (fraction)
		{
			fResult[i++] = 48 + fraction % 10;
			fraction /= 10;
		}

		while (i < maxDecimals)
			fResult[i++] = 48;

		fResult.Reverse();
		result += fResult;

		return result;
	}

	/// Converts the given double into a string.
	/// @returns The result.
	static Str<T, N> FromNum(const double num, const UInt_8 maxDecimals = 5)
	{
		SInt_64 whole = (SInt_64)num;

		Str<T, N> result;
		if (whole < 0)
			result += "-";

		result += Str<T, N>::FromNum(whole);

		UInt_64 power = 10;
		for (UInt_64 i = 0; i < (UInt_64)maxDecimals - 1; ++i)
			power *= 10;

		SInt_64 fraction = (SInt_64)((num - (double)whole) * (double)power);
		if (!fraction)
			return result;

		result += ".";

		Str<T, N> fResult(maxDecimals);
		N i = 0;

		while (fraction)
		{
			fResult[i++] = 48 + fraction % 10;
			fraction /= 10;
		}

		while (i < maxDecimals)
			fResult[i++] = 48;

		fResult.Reverse();
		result += fResult;

		return result;
	}

	/// Converts the given long double into a string.
	/// @returns The result.
	static Str<T, N> FromNum(const long double num, const UInt_8 maxDecimals = 5)
	{
		SInt_64 whole = (SInt_64)num;

		Str<T, N> result;
		if (whole < 0)
			result += "-";

		result += Str<T, N>::FromNum(whole);

		UInt_64 power = 10;
		for (UInt_64 i = 0; i < (UInt_64)maxDecimals - 1; ++i)
			power *= 10;

		SInt_64 fraction = (SInt_64)((num - (long double)whole) * (long double)power);
		if (!fraction)
			return result;

		result += ".";

		Str<T, N> fResult(maxDecimals);
		N i = 0;

		while (fraction)
		{
			fResult[i++] = 48 + fraction % 10;
			fraction /= 10;
		}

		while (i < maxDecimals)
			fResult[i++] = 48;

		fResult.Reverse();
		result += fResult;

		return result;
	}

	/// A 32-bit FNV-1a hash algorithm.
	/// @param [in] str The string to hash.
	/// @returns The resulting hash.
	static UInt_32 Hash_32(const Str<T, N>& str)
	{
		if (!str.Size())
			return 0;

		const Byte* const bytes = str.ToBytes();

		UInt_32 hash = 2166136261ul;

		for (N i = 0; i < str.Size(true); ++i)
			hash = (hash ^ bytes[i]) * 16777619;

		return hash;
	}

	UInt_32 Hash_32() const
	{
		if (!size)
			return 0;

		const Byte* const bytes = ToBytes();

		UInt_32 hash = 2166136261ul;

		for (N i = 0; i < Size(true); ++i)
			hash = (hash ^ bytes[i]) * 16777619;

		return hash;
	}

	/// A 64-bit FNV-1a hash algorithm.
	/// @param [in] str The string to hash.
	/// @returns The resulting hash.
	static UInt_64 Hash_64(const Str<T, N>& str)
	{
		if (!str.Size())
			return 0;

		const Byte* const bytes = str.ToBytes();

		UInt_64 hash = 14695981039346656037ull;

		for (N i = 0; i < str.Size(true); ++i)
			hash = (hash ^ bytes[i]) * 1099511628211;

		return hash;
	}

	UInt_64 Hash_64() const
	{
		if (!size)
			return 0;

		const Byte* const bytes = ToBytes();

		UInt_64 hash = 14695981039346656037ull;

		for (N i = 0; i < Size(true); ++i)
			hash = (hash ^ bytes[i]) * 1099511628211;

		return hash;
	}

	static N Len(const T* const str)
	{
		N count = 0;
		while (str[count])
			++count;
		return count;
	}

	static bool Cmp(const T* const a, const T* const b)
	{
		N aSize = Len(a);
		N bSize = Len(b);

		if (aSize != bSize)
			return false;

		for (UInt_64 i = 0; i < aSize; ++i)
			if (a[i] != b[i])
				return false;

		return true;
	}
};

typedef Str<Char_32, UInt_64> Str_32;
typedef Str<Char_16, UInt_64> Str_16;
typedef Str<Char_8, UInt_64> Str_8;

template<typename T = Char_8, typename N = UInt_64>
bool operator==(const T* const first, const Str<T, N>& second)
{
    N inSize = Str<T, N>::Len(first);
    if (second.Size() != inSize)
        return false;

    return Util::IsEqual(first, second, second.Size(true));
}

template<typename T = Char_8, typename N = UInt_64>
bool operator!=(const T* const first, const Str<T, N>& second)
{
	N inSize = Str<T, N>::Len(first);
	if (second.Size() != inSize)
		return true;

	return !Util::IsEqual(first, second, second.Size(true));
}

/// Concatenates a C-style string with a string.
/// @param [in] first The given C-style string.
/// @param [in] second The given string.
/// @returns The result.
template<typename T = Char_8, typename N = UInt_64>
Str<T, N> operator+(const T* const first, const Str<T, N>& second)
{
    N inSize = Str<T, N>::Len(first);

    Str<T, N> result(inSize + second.Size());

    Util::Copy(result, first, inSize * sizeof(T));
    Util::Copy(&result[inSize], &second[0], second.Size(true));

    result[inSize + second.Size()] = 0;

    return result;
}