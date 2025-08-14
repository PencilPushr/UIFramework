#pragma once

struct Colour
{
    float r, g, b, a;
    Colour() : r(0), g(0), b(0), a(1) {}
    Colour(float red, float green, float blue, float alpha = 1.0f)
        : r(red), g(green), b(blue), a(alpha) {}
};