#pragma once

#include <d3d11.h>

struct DX11
{
    // Data
    static ID3D11Device* Device;
    static ID3D11DeviceContext* DeviceContext;
    static IDXGISwapChain* SwapChain;
    static ID3D11RenderTargetView* RenderTargetView;
    static bool mIsValid;

    // Creation
    static bool CreateDeviceD3D(HWND hWnd);
    static void CreateRenderTarget();

    // Cleanup
    static void CleanupDeviceD3D();
    static void CleanupRenderTarget();
    static void ClearScreen(float r, float g, float b, float a);

    // Rendering
    static int Init(HWND);
    static void Render();
    static void Destroy();
};