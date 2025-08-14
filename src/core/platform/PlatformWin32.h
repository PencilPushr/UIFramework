#pragma once

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN 
#endif
#include <Windows.h> 
#include <string> 

#ifndef UNICODE
    #define UNICODE
#endif

#define TITLE L"UIFramework"

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct PlatformWin32
{
    // Static Win32 handles/state
    HINSTANCE Instance;
    HWND      Window;
    WNDCLASSEXW  WC;

    // Add callback for resize events (lets you ACTUALLY grab the width and height of the current renderer window)
    void(*ResizeCallback)(int width, int height) = nullptr;

    //PlatformWin32(const std::wstring& name, int x, int y, int w, int h);
    bool Init(const wchar_t* title, int x, int y, int w, int h);
    void Shutdown();
    bool ProcessMessages();
    void SetResizeCallback(void (*callback)(int, int)) { ResizeCallback = callback; }
};