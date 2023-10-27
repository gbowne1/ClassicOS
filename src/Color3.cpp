#include "Color3.h"

#include "Math.h"

Color3::Color3()
		: r(0.0f), g(0.0f), b(0.0f)
{
}

Color3::Color3(const float scalar)
		: r(Math::Clamp(scalar, 0.0f, 1.0f)), g(Math::Clamp(scalar, 0.0f, 1.0f)), b(Math::Clamp(scalar, 0.0f, 1.0f))
{
}

Color3::Color3(const float r, const float g, const float b)
		: r(Math::Clamp(r, 0.0f, 1.0f)), g(Math::Clamp(g, 0.0f, 1.0f)), b(Math::Clamp(b, 0.0f, 1.0f))
{
}

Color3::Color3(const Color3& color)
		: r(color.r), g(color.g), b(color.b)
{
}

Color3& Color3::operator=(const Color3& color)
{
	if (this == &color)
		return *this;

	r = color.r;
	g = color.g;
	b = color.b;

	return *this;
}

bool Color3::operator==(const Color3& color) const
{
	return r == color.r && g == color.g && b == color.b;
}

bool Color3::operator!=(const Color3& color) const
{
	return r != color.r || g != color.g || b != color.b;
}

float Color3::operator[](const UInt_64 i) const
{
	switch (i)
	{
		case 0:
			return r;
		case 1:
			return g;
		case 2:
			return b;
		default:
			return r;
	}
}

float& Color3::operator[](const UInt_64 i)
{
	switch (i)
	{
		case 0:
			return r;
		case 1:
			return g;
		case 2:
			return b;
		default:
			return r;
	}
}

Color3& Color3::operator*=(const Color3& color)
{
	r *= color.r;
	g *= color.g;
	b *= color.b;

	return *this;
}

Color3 Color3::operator*(const Color3& color) const
{
	return {r * color.r, g * color.g, b * color.b};
}