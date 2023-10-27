#pragma once


template<typename N>
class Range
{
public:
	N min;
	N max;

	Range()
		: min(0), max(0)
	{
	}

	Range(const N min, const N max)
		: min(min), max(max)
	{
	}

	Range(const Range& range)
		: min(range.min), max(range.max)
	{
	}
};