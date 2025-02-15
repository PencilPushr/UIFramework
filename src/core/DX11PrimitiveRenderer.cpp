// DX11PrimitiveRenderer.cpp
#include "DX11PrimitiveRenderer.h"
#include <d3dcompiler.h>
#include <cassert>
#include <cstring>

using namespace DirectX;

DX11PrimitiveRenderer::DX11PrimitiveRenderer(ID3D11Device* device, ID3D11DeviceContext* context)
    : m_device(device), m_context(context), m_vertexBuffer(nullptr)
{
    // Create a dynamic vertex buffer that can hold a reasonable number of vertices.
    HRESULT hr = CreateVertexBuffer();
    assert(SUCCEEDED(hr));
}

DX11PrimitiveRenderer::~DX11PrimitiveRenderer()
{
    if (m_vertexBuffer)
        m_vertexBuffer->Release();
}

HRESULT DX11PrimitiveRenderer::CreateVertexBuffer()
{
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.Usage = D3D11_USAGE_DYNAMIC;
    vbDesc.ByteWidth = sizeof(Vertex) * 1024;  // For example, space for 1024 vertices.
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    return m_device->CreateBuffer(&vbDesc, nullptr, &m_vertexBuffer);
}

void DX11PrimitiveRenderer::DrawBatch(D3D11_PRIMITIVE_TOPOLOGY topology)
{
    if (m_vertices.empty())
        return;

    // Map the dynamic vertex buffer.
    D3D11_MAPPED_SUBRESOURCE mapped;
    HRESULT hr = m_context->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    if (FAILED(hr))
        return;

    memcpy(mapped.pData, m_vertices.data(), sizeof(Vertex) * m_vertices.size());
    m_context->Unmap(m_vertexBuffer, 0);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
    m_context->IASetPrimitiveTopology(topology);

    m_context->Draw(static_cast<UINT>(m_vertices.size()), 0);

    m_vertices.clear();
}

void DX11PrimitiveRenderer::DrawLine(float x0, float y0, float x1, float y1, const Colour& color)
{
    m_vertices.push_back({ XMFLOAT3(x0, y0, 0.0f), XMFLOAT4(color.r, color.g, color.b, color.a) });
    m_vertices.push_back({ XMFLOAT3(x1, y1, 0.0f), XMFLOAT4(color.r, color.g, color.b, color.a) });
    DrawBatch(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void DX11PrimitiveRenderer::DrawTriangle(float x0, float y0, float x1, float y1, float x2, float y2, const Colour& color)
{
    m_vertices.push_back({ XMFLOAT3(x0, y0, 0.0f), XMFLOAT4(color.r, color.g, color.b, color.a) });
    m_vertices.push_back({ XMFLOAT3(x1, y1, 0.0f), XMFLOAT4(color.r, color.g, color.b, color.a) });
    m_vertices.push_back({ XMFLOAT3(x2, y2, 0.0f), XMFLOAT4(color.r, color.g, color.b, color.a) });
    DrawBatch(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void DX11PrimitiveRenderer::DrawRectangle(float x, float y, float width, float height, const Colour& color)
{
    // Define the four corners.
    Vertex v0 = { XMFLOAT3(x, y, 0.0f), XMFLOAT4(color.r, color.g, color.b, color.a) };
    Vertex v1 = { XMFLOAT3(x + width, y, 0.0f), XMFLOAT4(color.r, color.g, color.b, color.a) };
    Vertex v2 = { XMFLOAT3(x + width, y + height, 0.0f), XMFLOAT4(color.r, color.g, color.b, color.a) };
    Vertex v3 = { XMFLOAT3(x, y + height, 0.0f), XMFLOAT4(color.r, color.g, color.b, color.a) };

    // Two triangles: (v0,v1,v2) and (v0,v2,v3)
    m_vertices.push_back(v0);
    m_vertices.push_back(v1);
    m_vertices.push_back(v2);
    m_vertices.push_back(v0);
    m_vertices.push_back(v2);
    m_vertices.push_back(v3);
    DrawBatch(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void DX11PrimitiveRenderer::DrawCircle(float centerX, float centerY, float radius, const Colour& color, int segments)
{
    // Approximate the circle with a line strip.
    // (You might wish to use a triangle fan if you want a filled circle.)
    const float twoPi = 6.28318530718f;
    for (int i = 0; i <= segments; ++i)
    {
        float theta = (twoPi * i) / segments;
        float x = centerX + radius * cosf(theta);
        float y = centerY + radius * sinf(theta);
        m_vertices.push_back({ XMFLOAT3(x, y, 0.0f), XMFLOAT4(color.r, color.g, color.b, color.a) });
    }
    DrawBatch(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
}

void DX11PrimitiveRenderer::Flush()
{
    // In case you batch multiple primitives before drawing.
    // In our simple example, each draw call flushes immediately.
    if (!m_vertices.empty())
    {
        // Here you might choose an appropriate topology.
        DrawBatch(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }
}
