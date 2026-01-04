#include "Gui.h"


GUI::GUI()
{
	mCurState = STATE::VALID;
	mRun = true;

	if (!mWindow.Init(L"GUI", 300, 300, 600, 600))
	{
		fprintf(stderr, "Window failed to initialise\n");
		mCurState = STATE::ERR;
	}

	if (DX11::Init(mWindow.Window) != 0)
	{
		fprintf(stderr, "DX11 failed to intialise\n");
		mCurState = STATE::ERR;
	}

	mDXRenderer = new DX11PrimitiveRenderer(DX11::Device, DX11::DeviceContext);
	UIRenderer::SetRenderer(mDXRenderer);
}

GUI::~GUI()
{
	DX11::Destroy();
	mWindow.Shutdown();
}

void GUI::Render()
{
	UIRenderer::DrawLine(50, 50, 200, 200, Colour(1, 0, 0, 1));          // Red line.
	UIRenderer::DrawTriangle(300, 50, 350, 150, 250, 150, Colour(0, 1, 0, 1)); // Green triangle.
	UIRenderer::DrawRectangle(400, 50, 150, 100, Colour(0, 0, 1, 1));       // Blue rectangle.
	UIRenderer::DrawCircle(300, 300, 75, Colour(1, 1, 0, 1));               // Yellow circle.
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
