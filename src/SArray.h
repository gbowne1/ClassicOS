#pragma once

#include "EHS.h"

/// This container is useful for sorting arrays efficiently.
template<typename T, typename N = UInt_64>
class SArray
{
private:
	T* data;
	N size;

public:
	~SArray()
	{
		delete[] data;
	}

	SArray()
		: data(nullptr), size(0)
	{
	}

	SArray(const N size)
		: data(new T[size]), size(size)
	{
	}

	SArray(const SArray& sArray)
		: data(new T[sArray.size]), size(sArray.size)
	{
		for (N i = 0; i < size; ++i)
			data[i] = sArray.data[i];
	}

	SArray(SArray&& sArray) noexcept
		: data(sArray.data), size(sArray.size)
	{
		sArray.data = nullptr;
		sArray.size = 0;
	}

	SArray& operator=(const SArray& pArray)
	{
		if (this == &pArray)
			return *this;

		delete[] data;
		data = new T[pArray.size];
		for (N i = 0; i < pArray.size; ++i)
			data[i] = pArray.data[i];

		size = pArray.size;

		return *this;
	}

	SArray& operator=(SArray&& pArray) noexcept
	{
		if (this == &pArray)
			return *this;

		delete[] data;
		data = pArray.data;
		size = pArray.size;

		pArray.data = nullptr;
		pArray.size = 0;

		return *this;
	}

	operator const T* () const
	{
		return data;
	}

	operator T* ()
	{
		return data;
	}

	void Insert(const N index, T value)
	{
		if (index >= size)
		{
			return;
		}

		for (N i = size; i > index + 1; --i)
			data[i - 1] = std::move(data[i - 2]);

		data[index] = std::move(value);
	}

	void SetSize(const N newSize)
	{
		size = newSize;
	}

	N Size() const
	{
		return size;
	}
};