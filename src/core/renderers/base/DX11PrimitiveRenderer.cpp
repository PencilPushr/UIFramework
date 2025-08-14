// DX11PrimitiveRenderer.cpp
#include "DX11PrimitiveRenderer.h"
#include <d3dcompiler.h>
#include <cassert>
#include <cstring>

using namespace DirectX;

const char* vertexShaderSource = R"(
cbuffer ScreenData : register(b0)
{
    float2 ScreenDimensions;
    float2 Padding;  // Padding to make it 16-byte aligned
};

struct VS_INPUT
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    // Convert from pixel coordinates to NDC using actual screen dimensions
    output.Pos.x = (input.Pos.x / (ScreenDimensions.x * 0.5f)) - 1.0f;  // Map [0,width] to [-1,1]
    output.Pos.y = 1.0f - (input.Pos.y / (ScreenDimensions.y * 0.5f));  // Map [0,height] to [1,-1] (flip Y)
    output.Pos.z = 0.0f;
    output.Pos.w = 1.0f;
    output.Color = input.Color;
    return output;
}
)";

const char* pixelShaderSource = R"(
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

float4 PS(PS_INPUT input) : SV_TARGET
{
    return input.Color;
}
)";

DX11PrimitiveRenderer::DX11PrimitiveRenderer(ID3D11Device* device, ID3D11DeviceContext* context, int width, int height)
    : m_device(device), m_context(context), m_vertexBuffer(nullptr),
    m_vertexShader(nullptr), m_pixelShader(nullptr), m_inputLayout(nullptr),
    m_constantBuffer(nullptr), m_screenWidth(width), m_screenHeight(height)
{
    // Create vertex buffer
    HRESULT hr = CreateVertexBuffer();
    assert(SUCCEEDED(hr));

    // Create shaders and input layout
    hr = CreateShaders();
    assert(SUCCEEDED(hr));

    // Create constant buffer for screen dimensions
    hr = CreateConstantBuffer();
    assert(SUCCEEDED(hr));

    // Update the constant buffer with initial dimensions
    UpdateScreenDimensions(width, height);

    // Set up viewport
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (float) width;
    viewport.Height = (float) height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    m_context->RSSetViewports(1, &viewport);
}

DX11PrimitiveRenderer::~DX11PrimitiveRenderer()
{
    if (m_vertexBuffer) m_vertexBuffer->Release();
    if (m_vertexShader) m_vertexShader->Release();
    if (m_pixelShader) m_pixelShader->Release();
    if (m_inputLayout) m_inputLayout->Release();
    if (m_constantBuffer) m_constantBuffer->Release();
}

HRESULT DX11PrimitiveRenderer::CreateConstantBuffer()
{
    // Create constant buffer for screen dimensions
    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.ByteWidth = sizeof(ScreenData);
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    return m_device->CreateBuffer(&cbDesc, nullptr, &m_constantBuffer);
}

void DX11PrimitiveRenderer::UpdateScreenDimensions(int width, int height)
{
    m_screenWidth = width;
    m_screenHeight = height;

    // Update the constant buffer
    D3D11_MAPPED_SUBRESOURCE mapped;
    HRESULT hr = m_context->Map(m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    if (SUCCEEDED(hr))
    {
        ScreenData* data = (ScreenData*)mapped.pData;
        data->screenDimensions.x = (float)width;
        data->screenDimensions.y = (float)height;
        data->padding.x = 0.0f;
        data->padding.y = 0.0f;
        m_context->Unmap(m_constantBuffer, 0);
    }

    // Update viewport as well
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    m_context->RSSetViewports(1, &viewport);
}

int& DX11PrimitiveRenderer::GetWidth()
{
    return m_screenWidth;
}

int& DX11PrimitiveRenderer::GetHeight()
{
    return m_screenHeight;
}

HRESULT DX11PrimitiveRenderer::CreateShaders()
{
    HRESULT hr;

    // Compile vertex shader
    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    hr = D3DCompile(vertexShaderSource, strlen(vertexShaderSource), nullptr, nullptr, nullptr,
        "VS", "vs_4_0", 0, 0, &vsBlob, &errorBlob);
    if (FAILED(hr))
    {
        if (errorBlob)
        {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }
        return hr;
    }

    hr = m_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
        nullptr, &m_vertexShader);
    if (FAILED(hr))
    {
        vsBlob->Release();
        return hr;
    }

    // Create input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    hr = m_device->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(), &m_inputLayout);
    vsBlob->Release();
    if (FAILED(hr))
        return hr;

    // Compile pixel shader
    ID3DBlob* psBlob = nullptr;
    hr = D3DCompile(pixelShaderSource, strlen(pixelShaderSource), nullptr, nullptr, nullptr,
        "PS", "ps_4_0", 0, 0, &psBlob, &errorBlob);
    if (FAILED(hr))
    {
        if (errorBlob)
        {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }
        return hr;
    }

    hr = m_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(),
        nullptr, &m_pixelShader);
    psBlob->Release();

    return hr;
}

HRESULT DX11PrimitiveRenderer::CreateVertexBuffer()
{
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.Usage = D3D11_USAGE_DYNAMIC;
    vbDesc.ByteWidth = sizeof(Vertex) * 1024;  // Space for 1024 vertices
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    return m_device->CreateBuffer(&vbDesc, nullptr, &m_vertexBuffer);
}

void DX11PrimitiveRenderer::DrawBatch(D3D11_PRIMITIVE_TOPOLOGY topology)
{
    if (m_vertices.empty())
        return;

    // Map the dynamic vertex buffer
    D3D11_MAPPED_SUBRESOURCE mapped;
    HRESULT hr = m_context->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    if (FAILED(hr))
        return;

    memcpy(mapped.pData, m_vertices.data(), sizeof(Vertex) * m_vertices.size());
    m_context->Unmap(m_vertexBuffer, 0);

    // Set up the pipeline
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
    m_context->IASetPrimitiveTopology(topology);
    m_context->IASetInputLayout(m_inputLayout);

    // Set shaders
    m_context->VSSetShader(m_vertexShader, nullptr, 0);
    m_context->PSSetShader(m_pixelShader, nullptr, 0);

    // Set constant buffer for vertex shader
    m_context->VSSetConstantBuffers(0, 1, &m_constantBuffer);

    // Draw
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
    // Define the four corners
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
    // For a filled circle, use triangle fan approach
    std::vector<Vertex> circleVerts;

    // Center vertex
    circleVerts.push_back({ XMFLOAT3(centerX, centerY, 0.0f),
                           XMFLOAT4(color.r, color.g, color.b, color.a) });

    const float twoPi = 6.28318530718f;
    for (int i = 0; i <= segments; ++i)
    {
        float theta = (twoPi * i) / segments;
        float x = centerX + radius * cosf(theta);
        float y = centerY + radius * sinf(theta);
        circleVerts.push_back({ XMFLOAT3(x, y, 0.0f),
                               XMFLOAT4(color.r, color.g, color.b, color.a) });
    }

    // Convert to triangles
    for (int i = 0; i < segments; ++i)
    {
        m_vertices.push_back(circleVerts[0]);      // Center
        m_vertices.push_back(circleVerts[i + 1]);  // Current edge vertex
        m_vertices.push_back(circleVerts[i + 2]);  // Next edge vertex
    }

    DrawBatch(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void DX11PrimitiveRenderer::Flush()
{
    if (!m_vertices.empty())
    {
        DrawBatch(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }
}