// UIRenderer.h
#pragma once
#include "IPrimitiveRenderer.h"
#include "Colour.h"

class UIRenderer 
{
public:
    // Set the renderer. You can choose which backend to use here.
    static void SetRenderer(IPrimitiveRenderer* renderer) { s_renderer = renderer; }

    // High-level drawing functions:
    static void DrawLine(float x0, float y0, float x1, float y1, const Colour& color) 
    {
        if (s_renderer) s_renderer->DrawLine(x0, y0, x1, y1, color);
    }

    static void DrawTriangle(float x0, float y0, float x1, float y1, float x2, float y2, const Colour& color) 
    {
        if (s_renderer) s_renderer->DrawTriangle(x0, y0, x1, y1, x2, y2, color);
    }

    static void DrawRectangle(float x, float y, float width, float height, const Colour& color) 
    {
        if (s_renderer) s_renderer->DrawRectangle(x, y, width, height, color);
    }

    static void DrawCircle(float x, float y, float radius, const Colour& color, int segments = 36) 
    {
        if (s_renderer) s_renderer->DrawCircle(x, y, radius, color, segments);
    }

private:
    // The active primitive renderer.
    static IPrimitiveRenderer* s_renderer;
};
