#pragma once

class HSL
{
public:
    HSL(int h, float s, float l);
    float GetLuminance();
    static float Min(float a, float b);
    static float Max(float a, float b);
    static HSL RGBtoHSL(unsigned char r, unsigned char g, unsigned char b);

private:
    int h;
    float s;
    float l;
};