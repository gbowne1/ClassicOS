#pragma once

#include "EHS.h"

class Color4
{
public:
	float r;
	float g;
	float b;
	float a;

	Color4();

	Color4(const float scalar, const float a = 1.0f);

	Color4(const float r, const float g, const float b, const float a = 1.0f);

	Color4(const Color4& color);

	Color4& operator=(const Color4& color);

	bool operator==(const Color4& color) const;

	bool operator!=(const Color4& color) const;

	float operator[](const UInt_64 i) const;

	float& operator[](const UInt_64 i);

	Color4& operator*=(const Color4& color);

	Color4 operator*(const Color4& color) const;
};