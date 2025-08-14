#include "PlatformWin32.h"

#include <string>
#include "backend/DX11Renderer.h"

#define _IDIOTFIX

// Global pointer to access the platform instance from WndProc
static PlatformWin32* g_platformInstance = nullptr;

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
            int newWidth = (UINT)LOWORD(lParam);
            int newHeight = (UINT)HIWORD(lParam);

            DX11::CleanupRenderTarget();
            DX11::SwapChain->ResizeBuffers(0, newWidth, newHeight, DXGI_FORMAT_UNKNOWN, 0);
            DX11::CreateRenderTarget();

            // Call the resize callback if set
            if (g_platformInstance && g_platformInstance->ResizeCallback)
            {
                g_platformInstance->ResizeCallback(newWidth, newHeight);
            }
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
    // Set the global pointer so WndProc can access this instance
    g_platformInstance = this;

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
#ifdef _IDIOTFIX
    // ShowWindow returns nonzero if window was previously visible, 0 if previously hidden
    // For a new window, it returns 0 even on success, so we just call it and return true
    ShowWindow(Window, SW_SHOWDEFAULT);
    UpdateWindow(Window);  // Force an immediate paint

    return true;  // Return true since window creation succeeded
}
#else
    return ShowWindow(Window, SW_SHOWDEFAULT);
    // Don't call UpdateWindow as we don't have another handle/instance
}
#endif

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