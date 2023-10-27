#pragma once

#include "EHS.h"
#include "Link.h"

template<typename T, typename N = USize>
class LinkedList
{
private:
	Link<T>* start;
	Link<T>* end;
	N size;

public:
	~LinkedList()
	{
		delete start;
	}

	LinkedList()
		: size(0), start(nullptr), end(nullptr)
	{
	}

	LinkedList(const LinkedList& list)
		: start(nullptr), end(nullptr), size(list.size)
	{
		const Link<T>* rLast = list.start;
		Link<T>* last = new Link<T>(rLast->value);
		start = last;

		while (rLast->child)
		{
			last->child = new Link<T>(rLast->child->value);
			last = last->child;
			rLast = rLast->child;
		}

		end = last;
	}

	LinkedList(LinkedList&& list) noexcept
		: start(list.start), end(list.end), size(list.size)
	{
		list.start = nullptr;
		list.end = nullptr;
		list.size = {};
	}

	LinkedList& operator=(const LinkedList& list)
	{
		if (this == &list)
			return *this;

		const Link<T>* rLast = list.start;
		Link<T>* last = new Link<T>(rLast->value);
		start = last;

		while (rLast->child)
		{
			last->child = new Link<T>(rLast->child->value);
			last = last->child;
			rLast = rLast->child;
		}

		end = last;
		size = list.size;

		return *this;
	}

	LinkedList& operator=(LinkedList&& list) noexcept
	{
		if (this == &list)
			return *this;

		start = list.start;
		end = list.end;
		size = list.size;

		list.start = nullptr;
		list.end = nullptr;
		list.size = {};

		return *this;
	}

	const Link<T>* operator[](const N index) const
	{
		const Link<T>* result = start;

		for (N i = 0; i != index; ++i)
			result = result->child;

		return result;
	}

	Link<T>* operator[](const N index)
	{
		Link<T>* result = start;

		for (N i = 0; i != index; ++i)
			result = result->child;

		return result;
	}

	T& Insert(const N index, const T value)
	{
		if (index && index == size - 1)
		{
			end->child = new Link<T>(value);
			end = end->child;
			++size;
			return end->value;
		}
		else if (index)
		{
			Link<T>* hierarchy = start;
			for (N i = 0; i != index - 1; ++i)
				hierarchy = hierarchy->child;

			hierarchy->child = new Link<T>(value, hierarchy->child);
			++size;
			return hierarchy->child->value;
		}
		else
		{
			start = new Link<T>(value, start);
			++size;
			return start->value;
		}
	}

	T Remove(const N index)
	{
		if (index && index == size - 1)
		{
			Link<T>* hierarchy = start;
			while (hierarchy->child->child)
				hierarchy = hierarchy->child;

			T result = end->value;
			delete end;
			end = hierarchy;
			--size;
			return result;
		}
		else if (index)
		{
			Link<T>* hierarchy = start;
			for (N i = 0; i != index - 1; ++i)
				hierarchy = hierarchy->child;

			Link<T>* tmp = hierarchy->child;
			T result = tmp->value;
			hierarchy->child = hierarchy->child->child;
			tmp->child = nullptr;
			delete tmp;
			return result;
		}
		else
		{
			Link<T>* tmp = start;
			T result = tmp->value;
			start = start->child;

			if (--size)
				tmp->child = nullptr;
			else
				end = nullptr;

			delete tmp;

			return result;
		}
	}

	T& Push(const T value)
	{
		if (size)
		{
			end->child = new Link<T>(value);
			end = end->child;
			++size;
			return end->value;
		}
		else
		{
			start = new Link<T>(value);
			end = start;
			size = 1;
			return start->value;
		}
	}

	T Pop()
	{
		if (size == 1)
		{
			T result = start->value;
			delete start;
			start = nullptr;
			end = nullptr;
			size = 0;
			return result;
		}
		if (size > 1)
		{
			Link<T>* hierarchy = start;
			while (hierarchy->child->child)
				hierarchy = hierarchy->child;

			T result = hierarchy->child->value;
			delete hierarchy->child;
			hierarchy->child = nullptr;
			end = hierarchy;
			return result;
		}
		else
			return {};
	}

	void Clear()
	{
		delete start;
		start = nullptr;
		end = nullptr;
		size = 0;
	}

	N Size() const
	{
		return size;
	}
};