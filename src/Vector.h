#pragma once

#include "EHS.h"

#include <initializer_list>
#include <utility>

/// An array with extra memory pre-allocated for fast pushes.
/// @tparam T Array data type to use.
/// @tparam N Number data type to use.
/// @note If extra memory is set to five then each time that memory is filled it will add five extra.
template<typename T, typename N = UInt_64>
class Vector
{
protected:
	N rawSize;
	N size;
	N stride;
	T* data;

public:
	/// Frees any data created on the heap.
	~Vector()
	{
		delete[] data;
	}

	/// Default members initialization.
	Vector()
		: rawSize(0), size(0), stride(5), data(nullptr)
	{
	}

	/// Initializes members for pre-allocated memory to write to later.
	/// @param [in] size The size of memory to pre-allocate.
	/// @param [in] stride The stride size of memory to pre-allocate.
	Vector(const N size, const N stride)
		: rawSize(size + stride), size(size), stride(stride), data(new T[rawSize])
	{
	}

	/// Initializes this vector with an initializer list object.
	/// @param [in] list The given initializer list.
	/// @param [in] stride The extra amount of memory to allocate.
	Vector(std::initializer_list<T> list, const N stride = 5)
			: rawSize(0), size(list.size()), stride(stride), data(nullptr)
	{
		if (stride)
		{
			rawSize = list.size() / stride * stride;
			if (list.size() % stride)
				rawSize += stride;
		}
		else
		{
			rawSize = list.size();
		}

		data = new T[rawSize];

		N i = 0;
		for (auto v = list.begin(); v != list.end(); ++v)
			data[i++] = std::move(*v);
	}

	/// Initializes members with given C-style array.
	/// @param [in] data The C-style array.
	/// @param [in] size The size of the given C-style array.
	/// @param [in] stride The size of the extra memory allocated.
	Vector(const T* data, const N size, const N stride)
		: rawSize(0), size(size), stride(stride), data(nullptr)
	{
		if (stride)
		{
			rawSize = size / stride * stride;
			if (size % stride)
				rawSize += stride;
		}
		else
		{
			rawSize = size;
		}

		data = new T[rawSize];

		for (N i = 0; i < size; ++i)
			this->data[i] = data[i];
	}

	/// Copies all members from the given vector object.
	/// @param [in] vec The vector object to copy from.
	Vector(const Vector& vec)
		: rawSize(vec.rawSize), size(vec.size), stride(vec.stride), data(new T[rawSize])
	{
		for (N i = 0; i < size; ++i)
			data[i] = vec.data[i];
	}

	Vector(Vector&& vec) noexcept
		: rawSize(vec.rawSize), size(vec.size), stride(vec.stride), data(vec.data)
	{
		vec.rawSize = 0;
		vec.size = 0;
		vec.stride = 0;
		vec.data = nullptr;
	}

	/// Copies all members from the given vector object.
	/// @param [in] vec The vector object to copy from.
	/// @returns The vector that has been assigned to.
	Vector& operator=(const Vector& vec)
	{
		if (this == &vec)
			return *this;

		rawSize = vec.rawSize;
		size = vec.size;
		stride = vec.stride;

		delete[] data;
		data = new T[rawSize];

		for (N i = 0; i < size; ++i)
			data[i] = vec.data[i];

		return *this;
	}

	Vector& operator=(Vector&& vec) noexcept
	{
		if (this == &vec)
			return *this;

		rawSize = vec.rawSize;
		size = vec.size;
		stride = vec.stride;
		delete[] data;
		data = vec.data;

		vec.rawSize = 0;
		vec.size = 0;
		vec.stride = 0;
		vec.data = nullptr;

		return *this;
	}

	/// Adds a given initializer list at the end of the vector.
	/// @param [in] value The given initializer list to push to the end of the vector.
	Vector& operator+=(std::initializer_list<T> value)
	{
		if (size + value.size() >= rawSize)
		{
			if (stride)
			{
				rawSize = (size + value.size()) / stride * stride;
				if ((size + value.size()) % stride)
					rawSize += stride;
			}
			else
			{
				rawSize = size + value.size();
			}

			T* result = new T[rawSize];

			for (N i = 0; i < size; ++i)
				result[i] = std::move(std::move(data[i]));

			delete[] data;
			data = result;
		}

		for (auto v = value.begin(); v != value.end(); ++v)
			data[size++] = std::move(*v);

		return *this;
	}

	/// Adds a given value at the end of the vector.
	/// @param [in] value The given value to push to the end of the vector.
	Vector& operator+=(const T value)
	{
		if (size + 1 >= rawSize)
		{
			if (stride)
				rawSize = size + stride;
			else
				rawSize = size + 1;

			T* result = new T[rawSize];

			for (N i = 0; i < size; ++i)
				result[i] = std::move(data[i]);

			delete[] data;
			data = result;
		}

		data[size++] = std::move(value);

		return *this;
	}

	/// Retrieves the raw C-style array from casting an array object.
	operator T* () const
	{
		return data;
	}

	/// Retrieves the size of the vector object including the extra memory allocated.
	/// @returns The raw size.
	N RawSize() const
	{
		return rawSize;
	}

	/// Retrieves the size of the array not including the extra memory allocated.
	/// @returns The size.
	N Size() const
	{
		return size;
	}

	/// Retrieves the size of the extra memory allocated.
	/// @returns The extra size.
	N Stride() const
	{
		return stride;
	}

	N End() const
	{
		return size ? size - 1 : size;
	}

	/// Copies a vector object with offsets.
	/// @param [in] dstOffset The offset index to copy the given vector object to.
	/// @param [in] src The given vector object.
	/// @param [in] srcOffset The offset index from the given vector object to copy from.
	void Copy(const N dstOffset, Vector<T, N> src, const N srcOffset = 0)
	{
		for (N i = 0; i < src.Size() - srcOffset; ++i)
			data[i + dstOffset] = std::move(src[i + srcOffset]);
	}

	/// Copies a C-style array with offsets.
	/// @param [in] dstOffset The offset index to copy the given C-style array to.
	/// @param [in] src The given C-style array.
	/// @param [in] size The size from the given C-style array to copy.
	void Copy(const N dstOffset, const T* src, const N size)
	{
		for (N i = 0; i < size; ++i)
			data[i + dstOffset] = src[i];
	}

	/// Swaps two values in the vector.
	/// @param [in] a The first index to swap with.
	/// @param [in] b The second index to swap with.
	void Swap(N a, N b)
	{
		T tmp = std::move(data[a]);

		data[a] = std::move(data[b]);
		data[b] = std::move(tmp);
	}

	/// Inserts a value at a specified index that is available.
	/// @param [in] index The index to insert the value at.
	/// @param [in] value The given value to insert.
	void Insert(const N index, const T value)
	{
		N newSize = 0;
		if (index > size - 1)
			newSize = size + ((index + 1) - size);
		else
			newSize = size + 1;

		if (newSize >= rawSize)
		{
			if (stride)
				rawSize += newSize + stride;
			else
				rawSize = newSize;

			T* result = new T[rawSize];

			for (N i = 0; i < index; ++i)
				result[i] = std::move(data[i]);

			result[index] = std::move(value);

			for (N i = index; i < size; ++i)
				result[i + 1] = std::move(data[i]);

			delete[] data;
			data = result;
		}
		else
		{
			for (N i = index; i < size; ++i)
				data[i + 1] = std::move(data[i]);

			data[index] = std::move(value);
		}

		size += newSize;
	}

	/// Removes a value at a specified index.
	/// @param [in] index The index to remove the value at.
	/// @returns The removed data.
	T Remove(const N index)
	{
		T popped = {};

		if (!size || index >= size)
			return popped;

		popped = std::move(data[index]);

		if (!stride)
		{
			rawSize = size - 1;
			T* result = new T[rawSize];

			for (N i = 0; i < index; ++i)
				result[i] = std::move(data[i]);

			for (N i = index + 1; i < size; ++i)
				result[i - 1] = std::move(data[i]);

			delete[] data;
			data = result;
		}
		else if (rawSize - stride && size - 1 <= rawSize - stride)
		{
			rawSize -= stride;
			T* result = new T[rawSize];

			for (N i = 0; i < index; ++i)
				result[i] = std::move(data[i]);

			for (N i = index + 1; i < size; ++i)
				result[i - 1] = std::move(data[i]);

			delete[] data;
			data = result;
		}
		else
		{
			for (N i = index + 1; i < size; ++i)
				data[i - 1] = std::move(data[i]);
		}

		--size;

		return popped;
	}

	/// Adds a given C-style array at the end of the vector.
	/// @param [in] value The given C-style array to push to the end of the vector.
	/// @param [in] size The size of the given C-style array.
	void Push(const T* const value, const N size)
	{
		if (this->size + size >= rawSize)
		{
			if (stride)
			{
				rawSize = (this->size + size()) / stride * stride;
				if ((this->size + size) % stride)
					rawSize += stride;
			}
			else
			{
				rawSize = this->size + size;
			}

			T* result = new T[rawSize];

			for (N i = 0; i < size; ++i)
				result[i] = std::move(data[i]);

			delete[] data;
			data = result;
		}

		for (N i = 0; i < size; ++i)
			data[this->size + i] = value[i];

		this->size += size;
	}

	/// Adds a given vector object at the end of the vector.
	/// @param [in] value The given vector object to push to the end of the vector.
	void Push(Vector<T> value)
	{
		if (size + value.size >= rawSize)
		{
			if (stride)
			{
				rawSize = (size + value.size) / stride * stride;
				if ((size + value.size) % stride)
					rawSize += stride;
			}
			else
			{
				rawSize = size + value.size;
			}

			T* result = new T[rawSize];

			for (N i = 0; i < size; ++i)
				result[i] = std::move(data[i]);

			delete[] data;
			data = result;
		}

		for (N i = 0; i < value.size; ++i)
			data[size + i] = std::move(value.data[i]);

		size += value.size;
	}

	/// Adds a given initializer at the end of the vector.
	/// @param [in] value The given initializer list to push to the end of the vector.
	void Push(std::initializer_list<T> value)
	{
		if (size + value.size() >= rawSize)
		{
			if (stride)
			{
				rawSize = (size + value.size()) / stride * stride;
				if ((size + value.size()) % stride)
					rawSize += stride;
			}
			else
			{
				rawSize = size + value.size();
			}

			T* result = new T[rawSize];

			for (N i = 0; i < size; ++i)
				result[i] = std::move(data[i]);

			delete[] data;
			data = result;
		}

		for (auto v = value.begin(); v != value.end(); ++v)
			data[size++] = std::move(*v);
	}

	/// Adds a given value at the end of the vector.
	/// @param [in] value The given value to push to the end of the vector.
	void Push(T value)
	{
		if (size + 1 >= rawSize)
		{
			if (stride)
				rawSize += stride;
			else
				rawSize = size + 1;

			T* result = new T[rawSize];

			for (N i = 0; i < size; ++i)
				result[i] = std::move(data[i]);

			delete[] data;
			data = result;
		}

		data[size++] = std::move(value);
	}

	/// Much like the stack it pops a value at the end of the vector.
	/// @returns The removed data.
	T Pop()
	{
		T popped = {};

		if (!size)
			return popped;

		popped = std::move(data[--size]);

		if (!stride)
		{
			rawSize = size;
			T* result = new T[rawSize];

			for (N i = 0; i < size; ++i)
				result[i] = std::move(data[i]);

			delete[] data;
			data = result;
		}
		else if (rawSize - stride && size < rawSize - stride)
		{
			rawSize -= stride;
			T* result = new T[rawSize];

			for (N i = 0; i < size; ++i)
				result[i] = std::move(data[i]);

			delete[] data;
			data = result;
		}

		return popped;
	}

	/// Resizes the vector while keeping its alignment.
	/// @param [in] newSize The size to change to.
	void Resize(const N newSize)
	{
		if (newSize == size)
			return;

		if (stride)
		{
			rawSize = newSize / stride * stride;
			if (newSize % stride)
				rawSize += stride;
		}
		else
		{
			rawSize = newSize;
		}

		T* result = new T[rawSize];

		for (N i = 0; i < size && i < newSize; ++i)
			result[i] = std::move(data[i]);

		delete[] data;
		data = result;
		size = newSize;
	}

	/// Removes any extra allocated memory.
	void ExactSize()
	{
		if (!stride)
			return;

		stride = 0;

		if (size)
		{
			rawSize = size;
			T* result = new T[rawSize];

			for (N i = 0; i < size; ++i)
				result[i] = std::move(data[i]);

			delete[] data;
			data = result;
		}
		else
		{
			rawSize = 0;
			delete[] data;
			data = nullptr;
		}
	}

	/// Clears all values in the vector object.
	void Clear()
	{
		if (!size)
			return;

		rawSize = stride;
		size = 0;

		delete[] data;

		if (rawSize)
			data = new T[rawSize];
		else
			data = nullptr;
	}
};