#pragma once
#include "IPrimitiveRenderer.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

// Simple vertex format for our 2D primitives
struct Vertex
{
    DirectX::XMFLOAT3 pos;  // Position in 3D (z is typically 0 for 2D)
    DirectX::XMFLOAT4 color;
};

// Constant buffer structure for screen data
struct ScreenData
{
    DirectX::XMFLOAT2 screenDimensions;
    DirectX::XMFLOAT2 padding;  // Padding for 16-byte alignment
};

class DX11PrimitiveRenderer : public IPrimitiveRenderer
{
public:
    DX11PrimitiveRenderer(ID3D11Device* device, ID3D11DeviceContext* context, int width, int height);
    ~DX11PrimitiveRenderer();

    virtual void DrawLine(float x0, float y0, float x1, float y1, const Colour& color) override;
    virtual void DrawTriangle(float x0, float y0, float x1, float y1, float x2, float y2, const Colour& color) override;
    virtual void DrawRectangle(float x, float y, float width, float height, const Colour& color) override;
    virtual void DrawCircle(float x, float y, float radius, const Colour& color, int segments = 36) override;

    // Call this each frame after you submit your primitives
    void Flush();

    // Call this when the window is resized
    void UpdateScreenDimensions(int width, int height);
    int& GetWidth();
    int& GetHeight();

private:
    // The DirectX device and context
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_context;

    // Pipeline state objects
    ID3D11VertexShader* m_vertexShader;
    ID3D11PixelShader* m_pixelShader;
    ID3D11InputLayout* m_inputLayout;
    ID3D11Buffer* m_constantBuffer;

    // A dynamic vertex buffer used to batch primitives
    ID3D11Buffer* m_vertexBuffer;

    // Screen dimensions
    int m_screenWidth;
    int m_screenHeight;

    // We batch vertices here
    std::vector<Vertex> m_vertices;

    // Helper to update the dynamic vertex buffer and issue a draw call
    void DrawBatch(D3D11_PRIMITIVE_TOPOLOGY topology);

    // Create pipeline state objects
    HRESULT CreateShaders();

    // Create constant buffer
    HRESULT CreateConstantBuffer();

    // Ensure the vertex buffer is created
    HRESULT CreateVertexBuffer();
};