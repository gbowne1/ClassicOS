#pragma once

template<typename T>
class Link
{
public:
	T value;
	Link<T> *child;

	~Link()
	{
		delete child;
	}

	Link()
		: value(), child(nullptr)
	{
	}

	Link(const T value, Link* child)
		: value(value), child(child)
	{
	}

	Link(const T value)
		: value(value), child(nullptr)
	{
	}

	Link(const Link& link)
		: value(link.value), child(nullptr)
	{
	}

	Link(Link&& link) noexcept
		: value(link.value), child(link.child)
	{
		link.value = 0;
		link.child = nullptr;
	}

	Link& operator=(const Link& link)
	{
		if (this == &link)
			return *this;

		value = link.value;
		child = nullptr;

		return *this;
	}

	Link& operator=(Link&& link) noexcept
	{
		if (this == &link)
			return *this;

		value = link.value;
		child = link.child;

		link.value = 0;
		link.child = nullptr;

		return *this;
	}
};