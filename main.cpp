#include <iostream>

// Step 1: Setup up WinProc and WinMessage so that windows knows how to communicate with our program.

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

}

// Is this necessary? Why can we not call main? we are already processing the msg's in WndProc right?
// LRESULT WinApi WinMain()

int main()
{
	
}
