#pragma once

#include "EHS.h"

class Color3
{
public:
	float r;
	float g;
	float b;

	Color3();

	Color3(const float scalar);

	Color3(const float r, const float g, const float b);

	Color3(const Color3& color);

	Color3& operator=(const Color3& color);

	bool operator==(const Color3& color) const;

	bool operator!=(const Color3& color) const;

	float operator[](const UInt_64 i) const;

	float& operator[](const UInt_64 i);

	Color3& operator*=(const Color3& color);

	Color3 operator*(const Color3& color) const;
};