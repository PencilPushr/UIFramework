#pragma once

#include "backend/DX11Renderer.h"
#include "platform/PlatformWin32.h"
#include "base/DX11PrimitiveRenderer.h"
#include "UIRenderer.h"

enum class STATE
{
	VALID = 0,
	ERR
};

class GUI
{
public:

	GUI();
	~GUI();
	void OnWindowResize(int newWidth, int newHeight);
	void Render();
	void Run();

private:
	bool mRun;
	STATE mCurState;
	PlatformWin32 mWindow;
	DX11PrimitiveRenderer* mDXRenderer;

	int mWindowWidth;
	int	mWindowHeight;
};