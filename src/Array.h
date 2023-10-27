#pragma once

#include "EHS.h"

#include <initializer_list>
#include <algorithm>

/// A helper class for C-style arrays.
/// @tparam T Array data type to use.
/// @tparam N Number data type to use.
template<typename T, typename N = USize>
class Array
{
protected:
	T* data;
	N size;

public:
	/// Frees any data created on the heap.
	~Array()
	{
		delete[] data;
	}

	/// Default members initialization.
	Array()
		: data(nullptr), size(0)
	{
	}

	/// Initializes an empty array with the given size.
	/// @note Data must be assigned manually using an index.
	explicit Array(const N size)
			: data(new T[size]), size(size)
	{
	}

	/// Initializes this array with an initializer list object.
	/// @param [in] list The given initializer list.
	Array(std::initializer_list<T> list)
			: data(new T[list.size()]), size(list.size())
	{
		N i = 0;
		for (auto v = list.begin(); v != list.end(); ++v)
			data[i++] = std::move(*v);
	}

	/// Initializes members with given C-style array.
	/// @param [in] data The C-style array.
	/// @param [in] size The size of the given C-style array.
	Array(const T* const data, const N size)
			: data(new T[size]), size(size)
	{
		for (N i = 0; i < size; ++i)
			this->data[i] = data[i];
	}

	/// Copies all members from the given array object.
	/// @param [in] array The array object to copy from.
	Array(const Array& array)
			: data(new T[array.size]), size(array.size)
	{
		for (N i = 0; i < size; ++i)
			data[i] = array.data[i];
	}

	Array(Array&& array) noexcept
		: data(array.data), size(array.size)
	{
		array.data = nullptr;
		array.size = 0;
	}

	/// Copies all members from the given array object.
	/// @param [in] array The array object to copy from.
	/// @returns The array that has been assigned to.
	Array<T, N>& operator=(const Array& array)
	{
		if (this == &array)
			return *this;

		delete[] data;
		data = new T[array.size];
		for (N i = 0; i < array.size; ++i)
			data[i] = array.data[i];

		size = array.size;

		return *this;
	}

	Array<T, N>& operator=(Array&& array) noexcept
	{
		if (this == &array)
			return *this;

		delete[] data;
		data = array.data;
		size = array.size;

		array.data = nullptr;
		array.size = 0;

		return *this;
	}

	/// Copies all members from the given initializer list object.
	/// @param [in] list The initializer list object to copy from.
	/// @returns The array that has been assigned to.
	Array& operator=(std::initializer_list<T> list)
	{
		delete[] data;
		data = new T[list.size];

		N i = 0;
		for (auto v = list.begin(); v != list.end(); ++v)
			data[i++] = std::move(*v);

		size = list.size();

		return *this;
	}

	/// Adds a given array object at the end of the array.
	/// @param [in] value The given array object to push to the end of the array.
	Array& operator+=(Array value)
	{
		T* result = new T[size + value.size];

		for (N i = 0; i < size; ++i)
			result[i] = std::move(data[i]);

		for (N i = 0; i < value.size; ++i)
			result[size + i] = std::move(value[i]);

		delete[] data;

		data = result;
		size += value.size;

		return *this;
	}

	bool operator==(const Array<T, N>& array) const
	{
		if (size != array.size)
			return false;

		for (N i = 0; i < size; ++i)
			if (data[i] != array[i])
				return false;

		return true;
	}

	bool operator!=(const Array<T, N>& array) const
	{
		if (size == array.size)
			return false;

		for (N i = 0; i < size; ++i)
			if (data[i] != array[i])
				return true;

		return false;
	}

	/// Adds a given array object at the end of the array.
	/// @param [in] value The given initializer list to push to the end of the array.
	Array& operator+=(std::initializer_list<T> value)
	{
		T* result = new T[size + value.size()];

		for (N i = 0; i < size; ++i)
			result[i] = std::move(data[i]);

		N i = 0;
		for (auto v = value.begin(); v != value.end(); ++v)
			result[size + i++] = std::move(*v);

		delete[] data;

		data = result;
		size += value.size();

		return *this;
	}

	/// Adds a given value at the end of the array.
	/// @param [in] value The given value to push to the end of the array.
	Array& operator+=(const T value)
	{
		T* result = new T[size + 1];

		for (N i = 0; i < size; ++i)
			result[i] = std::move(data[i]);

		result[size] = std::move(value);

		delete[] data;

		data = result;
		++size;

		return *this;
	}

	/// Retrieves the raw C-style array from casting an array object.
	operator T* () const
	{
		return data;
	}

	/// Swaps two values in the array.
	/// @param a The first index to swap with.
	/// @param b The second index to swap with.
	void Swap(N a, N b) const
	{
		T tmp = std::move(data[a]);

		data[a] = std::move(data[b]);
		data[b] = std::move(tmp);
	}

	/// Adds a given C-style array at the end of the array.
	/// @param [in] value The given C-style array to push to the end of the array.
	/// @param [in] size The size of the given C-style array.
	void Push(const T* const value, const N size)
	{
		T* result = new T[this->size + size];

		for (N i = 0; i < this->size; ++i)
			result[i] = std::move(this->data[i]);

		for (N i = 0; i < size; ++i)
			result[this->size + i] = value[i];

		delete[] data;

		this->data = result;
		this->size += size;
	}

	/// Adds a given array object at the end of the array.
	/// @param [in] value The given array object to push to the end of the array.
	void Push(Array value)
	{
		T* result = new T[size + value.size];

		for (N i = 0; i < size; ++i)
			result[i] = std::move(data[i]);

		for (N i = 0; i < value.size; ++i)
			result[size + i] = std::move(value[i]);

		delete[] data;

		data = result;
		size += value.size;
	}

	/// Adds a given array object at the end of the array.
	/// @param [in] value The given initializer list to push to the end of the array.
	void Push(std::initializer_list<T> value)
	{
		T* result = new T[size + value.size()];

		for (N i = 0; i < size; ++i)
			result[i] = std::move(data[i]);

		N i = 0;
		for (auto v = value.begin(); v != value.end(); ++v)
			result[size + i++] = std::move(*v);

		delete[] data;

		data = result;
		size += value.size();
	}

	/// Adds a given value at the end of the array.
	/// @param [in] value The given value to push to the end of the array.
	void Push(T value)
	{
		T* result = new T[size + 1];

		for (N i = 0; i < size; ++i)
			result[i] = std::move(data[i]);

		result[size] = std::move(value);

		delete[] data;

		data = result;
		++size;
	}

	/// Removes a value at the end of the array.
	/// @returns The value that was popped.
	T Pop()
	{
		T* result = new T[--size];

		T value = std::move(data[size]);

		for (N i = 0; i < size; ++i)
			result[i] = std::move(data[i]);

		delete[] data;

		data = result;

		return value;
	}

	/// Removes a value at a provided index.
	/// @param [in] index The index of the value to remove.
	/// @returns The value that was removed.
	T Remove(const N index)
	{
		T* result = new T[--size];

		T value = std::move(data[index]);

		for (N i = 0, c = 0; i < size; ++i)
			if (i != index)
				result[c++] = std::move(data[i]);

		delete[] data;

		data = result;

		return value;
	}

	void Clear()
	{
		if (!data)
			return;

		delete[] data;
		data = nullptr;
		size = 0;
	}

	/// Resizes the array.
	/// @param [in] newSize The size to change to.
	void Resize(const N newSize)
	{
		if (size == newSize)
			return;

		T* result = new T[newSize];

		for (N i = 0; i < newSize && i < size; ++i)
			result[i] = std::move(data[i]);

		delete[] data;

		data = result;
		size = newSize;
	}

	/// Retrieves the size of the array.
	/// @returns The resulting size.
	N Size() const
	{
		return size;
	}

	N End() const
	{
		return size - 1;
	}
};