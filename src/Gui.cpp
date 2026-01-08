#include "Gui.h"
/*
static GUI* g_guiInstance = nullptr;

// Static callback function - unfortunately this is required, as static methods cannot access private members
// so we require a wrapper over this, so that it can be access in the Renderer (GLRenderer, DXRenderer...)
static void WindowResizeCallback(int width, int height)
{
	if (g_guiInstance)
	{
		g_guiInstance->OnWindowResize(width, height);
	}
}

GUI::GUI()
{
	mCurState = STATE::VALID;
	mRun = true;
	g_guiInstance = this;

	// Store window dimensions
	mWindowWidth = 600;
	mWindowHeight = 600;

	// Idiotic - returning the value of ShowWindow, even on success is 0. Meaning this would fail, so we return true; inside Init()
	if (!mWindow.Init(L"GUI", 300, 300, mWindowWidth, mWindowHeight))
	{
		fprintf(stderr, "Window failed to initialise\n");
		mCurState = STATE::ERR;
	}

	if (DX11::Init(mWindow.Window) != 0)
	{
		fprintf(stderr, "DX11 failed to intialise\n");
		mCurState = STATE::ERR;
	}

	mDXRenderer = new DX11PrimitiveRenderer(DX11::Device, DX11::DeviceContext, mWindowWidth, mWindowHeight);
	UIRenderer::SetRenderer(mDXRenderer);
	mWindow.SetResizeCallback(WindowResizeCallback);
}

GUI::~GUI()
{
	g_guiInstance = nullptr;  // Clear global pointer
	delete mDXRenderer;
	DX11::Destroy();
	mWindow.Shutdown();
}

void GUI::Render()
{
	//UIRenderer::DrawLine(50, 50, 200, 200, Colour(1, 0, 0, 1));          // Red line.
	//UIRenderer::DrawTriangle(300, 50, 350, 150, 250, 150, Colour(0, 1, 0, 1)); // Green triangle.
	//UIRenderer::DrawRectangle(400, 50, 150, 100, Colour(0, 0, 1, 1));       // Blue rectangle.
	//UIRenderer::DrawCircle(300, 300, 75, Colour(1, 1, 0, 1));               // Yellow circle.

	using ui = UIRenderer;

	// Get current dimensions from renderer
	float centerX = mDXRenderer->GetWidth() / 2.f;
	float centerY = mDXRenderer->GetHeight() / 2.f;

	// Draw eyes
	ui::DrawCircle(centerX - 60, centerY - 50, 20, Colour(0, 0, 1, 1));
	ui::DrawCircle(centerX + 60, centerY - 50, 20, Colour(0, 0, 1, 1));

	// Draw smile
	ui::DrawLine(centerX - 50, centerY + 10, centerX - 20, centerY + 30, Colour(0, 0, 1, 1));
	ui::DrawLine(centerX - 20, centerY + 30, centerX + 20, centerY + 30, Colour(0, 0, 1, 1));
	ui::DrawLine(centerX + 20, centerY + 30, centerX + 50, centerY + 10, Colour(0, 0, 1, 1));
}

void GUI::OnWindowResize(int newWidth, int newHeight)
{
	mWindowWidth = newWidth;
	mWindowHeight = newHeight;
	if (mDXRenderer) 
	{
		mDXRenderer->UpdateScreenDimensions(newWidth, newHeight);
	}
}

void GUI::Run()
{
	while (mWindow.ProcessMessages() && mRun)
	{
		switch (mCurState)
		{

			case STATE::VALID:
			{
				// Clear screen (with an alpha of 0 for transparency, if needed)
				DX11::ClearScreen(0.7f, 1.f, 1.f, 1.0f);

				// Render main items
				Render();

				// Render your content/primitives
				DX11::Render();
			} break;

			case STATE::ERR:
			default:
				mRun = false;
				break;
		}
	}
}
*/