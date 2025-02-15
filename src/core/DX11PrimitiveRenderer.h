#pragma once
#include "IPrimitiveRenderer.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

// Simple vertex format for our 2D primitives.
struct Vertex
{
    DirectX::XMFLOAT3 pos;  // Position in 3D (z is typically 0 for 2D).
    DirectX::XMFLOAT4 color;
};

class DX11PrimitiveRenderer : public IPrimitiveRenderer 
{
public:
    DX11PrimitiveRenderer(ID3D11Device* device, ID3D11DeviceContext* context);
    ~DX11PrimitiveRenderer();

    virtual void DrawLine(float x0, float y0, float x1, float y1, const Colour& color) override;
    virtual void DrawTriangle(float x0, float y0, float x1, float y1, float x2, float y2, const Colour& color) override;
    virtual void DrawRectangle(float x, float y, float width, float height, const Colour& color) override;
    virtual void DrawCircle(float x, float y, float radius, const Colour& color, int segments = 36) override;

    // Call this each frame after you submit your primitives.
    void Flush();

private:
    // The DirectX device and context.
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_context;

    // A dynamic vertex buffer used to batch primitives.
    ID3D11Buffer* m_vertexBuffer;

    // We batch vertices here; you can choose to flush when full.
    std::vector<Vertex> m_vertices;

    // Helper to update the dynamic vertex buffer and issue a draw call.
    void DrawBatch(D3D11_PRIMITIVE_TOPOLOGY topology);

    // Ensure the vertex buffer is created.
    HRESULT CreateVertexBuffer();
};