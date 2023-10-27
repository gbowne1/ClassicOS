#include "Color4.h"

#include "Math.h"


{
    Color4::Color4()
        : r(0.0f), g(0.0f), b(0.0f), a(1.0f)
    {
    }

    Color4::Color4(const float scalar, const float a)
        : r(Math::Clamp(scalar, 0.0f, 1.0f)), g(Math::Clamp(scalar, 0.0f, 1.0f)), b(Math::Clamp(scalar, 0.0f, 1.0f)), a(a)
    {
    }

    Color4::Color4(const float r, const float g, const float b, const float a)
        : r(Math::Clamp(r, 0.0f, 1.0f)), g(Math::Clamp(g, 0.0f, 1.0f)), b(Math::Clamp(b, 0.0f, 1.0f)), a(Math::Clamp(a, 0.0f, 1.0f))
    {
    }

    Color4::Color4(const Color4& color)
        : r(color.r), g(color.g), b(color.b), a(color.a)
    {
    }

    Color4& Color4::operator=(const Color4& color)
    {
        if (this == &color)
            return *this;

        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;

        return *this;
    }

	bool Color4::operator==(const Color4& color) const
	{
		return r == color.r && g == color.g && b == color.b && a == color.a;
	}

	bool Color4::operator!=(const Color4& color) const
	{
		return r != color.r || g != color.g || b != color.b || a != color.a;
	}

    float Color4::operator[](const UInt_64 i) const
    {
        switch (i)
        {
        case 0:
            return r;
        case 1:
            return g;
        case 2:
            return b;
        case 3:
            return a;
        default:
            return r;
        }
    }

    float& Color4::operator[](const UInt_64 i)
    {
        switch (i)
        {
        case 0:
            return r;
        case 1:
            return g;
        case 2:
            return b;
        case 3:
            return a;
        default:
            return r;
        }
    }

	Color4& Color4::operator*=(const Color4& color)
	{
		r *= color.r;
		g *= color.g;
		b *= color.b;

		return *this;
	}

	Color4 Color4::operator*(const Color4& color) const
	{
		return {r * color.r, g * color.g, b * color.b, a};
	}
}