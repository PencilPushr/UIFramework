#include "DX11Renderer.h"

ID3D11Device* DX11::Device = nullptr;
ID3D11DeviceContext* DX11::DeviceContext = nullptr;
IDXGISwapChain* DX11::SwapChain = nullptr;
ID3D11RenderTargetView* DX11::RenderTargetView = nullptr;
bool DX11::mIsValid = false;

#include <stdio.h>

int DX11::Init(HWND window)
{
    if (CreateDeviceD3D(window) == false)
    {
        CleanupDeviceD3D();
        mIsValid = false;
        return 1;
    }
    return 0;
}

void DX11::Destroy()
{
    // We don't want to try and perform cleanup if we already failed.
    if (!(mIsValid))
    {
        CleanupDeviceD3D();
    }
}

void DX11::Render()
{
    DeviceContext->OMSetRenderTargets(1, &RenderTargetView, nullptr);

    //g_pSwapChain->Present(1, 0); // Present with vsync
    SwapChain->Present(0, 0); // Present without vsync
}

void DX11::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (SwapChain)      { SwapChain->Release();     SwapChain = nullptr; }
    if (DeviceContext)  { DeviceContext->Release(); DeviceContext = nullptr; }
    if (Device)         { Device->Release();        Device = nullptr; }
}

void DX11::CleanupRenderTarget()
{
    if (RenderTargetView) { RenderTargetView->Release(); RenderTargetView = nullptr; }
}

void DX11::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    SwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    Device->CreateRenderTargetView(pBackBuffer, nullptr, &RenderTargetView);
    pBackBuffer->Release();
}

bool DX11::CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    
    HRESULT res = D3D11CreateDeviceAndSwapChain(
        nullptr, 
        D3D_DRIVER_TYPE_HARDWARE, 
        nullptr, 
        createDeviceFlags, 
        featureLevelArray, 
        2, 
        D3D11_SDK_VERSION, 
        &sd, 
        &SwapChain, 
        &Device, 
        &featureLevel, 
        &DeviceContext
    );

    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(
            nullptr, 
            D3D_DRIVER_TYPE_WARP, 
            nullptr, 
            createDeviceFlags, 
            featureLevelArray, 
            2, 
            D3D11_SDK_VERSION,
            &sd, 
            &SwapChain, 
            &Device, 
            &featureLevel, 
            &DeviceContext
        );
    
    if (FAILED(res))
    {
        // Get a human-readable string for the error.
        LPVOID errMsg;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            res,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&errMsg,
            0,
            nullptr
        );
        fprintf(stderr, "D3D11CreateDeviceAndSwapChain failed with error 0x%08X: %s\n", res, (LPWSTR)errMsg);
        LocalFree(errMsg);
        return false;
    }

    CreateRenderTarget();
    return true;
}

void DX11::ClearScreen(float r, float g, float b, float a)
{
    float rgba[4] = { r,g,b,a };
    DX11::DeviceContext->ClearRenderTargetView(DX11::RenderTargetView, rgba);
}