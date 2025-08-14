#pragma once
#include "Colour.h"

class IPrimitiveRenderer 
{
public:
    virtual ~IPrimitiveRenderer() {}

    // Draw a line from (x0, y0) to (x1, y1)
    virtual void DrawLine(float x0, float y0, float x1, float y1, const Colour& color) = 0;

    // Draw a triangle defined by three points.
    virtual void DrawTriangle(float x0, float y0, float x1, float y1, float x2, float y2, const Colour& color) = 0;

    // Draw a rectangle with the top-left corner at (x, y) and given width and height.
    virtual void DrawRectangle(float x, float y, float width, float height, const Colour& color) = 0;

    // Draw a circle centered at (x, y) with the given radius.
    virtual void DrawCircle(float x, float y, float radius, const Colour& color, int segments = 36) = 0;
};