#include "PlatformWin32.h"

#include <string>
#include "backend/DX11Renderer.h"

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    switch (msg)
    {
    case WM_ERASEBKGND:
        // Tell Windows that the background has been erased.
        return 1;
    case WM_SIZE:
        if (DX11::Device != nullptr && wParam != SIZE_MINIMIZED)
        {   
            DX11::CleanupRenderTarget();
            DX11::SwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            DX11::CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT Simulation menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

// Essentially the wide char version and can specify where (x,y) we want to spawn it on the screen
bool PlatformWin32::Init(const wchar_t* title, int x, int y, int w, int h)
{
    // Fill in the member variable WC
    ZeroMemory(&WC, sizeof(WNDCLASSEXW));
    WC.cbSize = sizeof(WNDCLASSEXW);
    WC.style = CS_HREDRAW | CS_VREDRAW;
    WC.lpfnWndProc = WndProc;
    WC.cbClsExtra = 0;
    WC.cbWndExtra = 0;
    WC.hInstance = GetModuleHandle(nullptr);
    WC.hIcon = LoadIcon(nullptr, IDI_APPLICATION); // Valid icon
    WC.hCursor = LoadCursor(nullptr, IDC_ARROW);     // Valid cursor
    WC.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);             // Valid background brush
    WC.lpszMenuName = nullptr;
    WC.lpszClassName = L"UIFrameworkClass";                  // Use a consistent class name
    WC.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    
    if (!RegisterClassExW(&WC))
    {
        fprintf(stderr, "Failed to register window class! \n");
        return false;
    }

    Window = CreateWindowW(WC.lpszClassName, title, WS_OVERLAPPEDWINDOW, x, y, w, h, nullptr, nullptr, WC.hInstance, nullptr);
    Instance = WC.hInstance; // GetModuleHandle(nullptr)

    if (!Window)
    {
        fprintf(stderr, "Failed to register window class! \n");
        return false;
    }

    ShowWindow(Window, SW_SHOWDEFAULT);
    UpdateWindow(Window);

    return true;
}

void PlatformWin32::Shutdown()
{
    // If needed, destroy window, unregister class, etc.
    if (Window) 
    {
        DestroyWindow(Window);
        Window = nullptr;
    }
    UnregisterClassW(WC.lpszClassName, Instance);
}

bool PlatformWin32::ProcessMessages()
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
    {
        if (msg.message == WM_QUIT)
            return false;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}