#include "HSL.hpp"

HSL::HSL(int h, float s, float l)
{
    this->h = h;
    this->s = s;
    this->l = l;
}

float HSL::GetLuminance()
{
    return l;
}

float HSL::Min(float a, float b)
{
    return a <= b ? a : b;
}

float HSL::Max(float a, float b)
{
    return a >= b ? a : b;
}

HSL HSL::RGBtoHSL(unsigned char red, unsigned char green, unsigned char blue)
{
    HSL hsl = HSL(0, 0, 0);

    float r = (red / 255.0f);
    float g = (green / 255.0f);
    float b = (blue / 255.0f);

    float min = Min(Min(r, g), b);
    float max = Max(Max(r, g), b);
    float delta = max - min;

    hsl.l = (max + min) / 2;

    if (delta == 0)
    {
        hsl.h = 0;
        hsl.s = 0.0f;
    }
    else
    {
        hsl.s = (hsl.l <= 0.5) ? (delta / (max + min)) : (delta / (2 - max - min));

        float hue;

        if (r == max)
        {
            hue = ((g - b) / 6) / delta;
        }
        else if (g == max)
        {
            hue = (1.0f / 3) + ((b - r) / 6) / delta;
        }
        else
        {
            hue = (2.0f / 3) + ((r - g) / 6) / delta;
        }

        if (hue < 0)
            hue += 1;
        if (hue > 1)
            hue -= 1;

        hsl.h = (int)(hue * 360);
    }

    return hsl;
}