# UIFramework

2D primitive rendering framework, designed for cross-compatability by writing backend components via "renderers and platforms". This projectis meant for easy prototyping of 2D applications.

Currently only supporting DirectX 11 and Win32. OpenGL and GLFW soon!

## In action:
<img width="604" height="604" alt="image" src="https://github.com/user-attachments/assets/0d193fde-921d-4da5-b5d9-7c7eeebbe04e" />


# Highly recommended resources
ChiliTomatoNoodle - DirectX 11 series  
Game Engine Series by The Cherno - Renderer architecture  
jpres - Low-level graphics programming  


https://learnopengl.com/ <- This one is not a directx related but possibly the most useful one  
http://www.directxtutorial.com/Lesson.aspx?lessonid=112-1-1  
https://www.rastertek.com/dx11win10tut02.html  

# Features

Cross-platform abstraction: Clean separation between platform layer and rendering  
DirectX 11 backend: Hardware-accelerated 2D primitive rendering  
Simple API: Easy-to-use drawing functions for basic shapes  
Real-time rendering: Immediate mode rendering with dynamic vertex buffers  

# Supported Primitives

Lines
Triangles
Rectangles
Circles (with configurable segments)

# Project Structure
```
├── backend/
│   ├── DX11Renderer.h/cpp          # DirectX 11 device management
│   └── DX11PrimitiveRenderer.h/cpp # Primitive rendering implementation
├── platform/
│   └── PlatformWin32.h/cpp         # Win32 window management
├── renderers/
|   ├── IPrimitiveRenderer.h            # Renderer interface
|   └── UIRenderer.h/cpp                # High-level rendering API
├── Colour.h                        # Color structure
├── Gui.h/cpp                       # Main application class
└── main.cpp                        # Entry point
```
# Usage example:
```cpp
#include "UIRenderer.h"

// Initialize renderer (done in GUI constructor)
DX11PrimitiveRenderer* renderer = new DX11PrimitiveRenderer(device, context, width, height);
UIRenderer::SetRenderer(renderer);

// Draw primitives
UIRenderer::DrawLine(0, 0, 100, 100, Colour(1, 0, 0, 1));           // Red line
UIRenderer::DrawTriangle(50, 50, 100, 150, 0, 150, Colour(0, 1, 0, 1)); // Green triangle
UIRenderer::DrawRectangle(200, 50, 100, 75, Colour(0, 0, 1, 1));    // Blue rectangle
UIRenderer::DrawCircle(300, 200, 50, Colour(1, 1, 0, 1));           // Yellow circle
```

# Architecture
The framework uses a layered architecture:

1. Platform Layer (PlatformWin32): Handles window creation, message pumping, and OS-specific functionality
2. Graphics Backend (DX11Renderer): Manages DirectX 11 device, swap chain, and render targets
3. Primitive Renderer (DX11PrimitiveRenderer): Implements shape drawing using shaders and vertex buffers
4. High-Level API (UIRenderer): Provides simple static functions for drawing primitives
5. Application Layer (GUI): Coordinates all systems and implements the main render loop


# Color Format
Colors use RGBA format with float values from 0.0 to 1.0:
```cpp
Colour red(1.0f, 0.0f, 0.0f, 1.0f);  // Red, full opacity
```

# Known limitations
- Immediate mode rendering (not optimized for large numbers of primitives)
- No text rendering
- No texture support
- Window resizing may require manual handling for proper scaling

Currently it also only supports:
- Windows only (Win32 dependency, GLFW not yet supported)
- DirectX 11 only (no OpenGL/Vulkan backends)

# TODO:
- OpenGL backend for cross-platform support
- Texture/sprite rendering
- Text rendering capabilities
- Better batching for improved performance
- Input handling system
- Audio integration
- Scene graph/entity system

# For educational purposes:

## Coordinate System
- Origin (0,0) is at the top-left corner
- X increases rightward, Y increases downward
- All coordinates are in pixels

## Rendering Pipeline
1. Clear the render target
2. Set up shaders and vertex buffers
3. Transform pixel coordinates to normalized device coordinates (NDC)
4. Draw primitives using immediate mode batching
5. Present the frame

