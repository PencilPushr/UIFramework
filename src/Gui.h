#pragma once

#include "backend/DX11Renderer.h"
#include "platform/PlatformWin32.h"
#include "DX11PrimitiveRenderer.h"
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
	void Render();
	void Run();

private:
	bool mRun;
	STATE mCurState;
	PlatformWin32 mWindow;
	DX11PrimitiveRenderer* mDXRenderer;
};