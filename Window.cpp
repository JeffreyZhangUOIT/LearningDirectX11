#include "Window.h"
#include <Windows.h>

LRESULT CALLBACK WinProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam) 
{
	if (msg == WM_DESTROY || msg == WM_CLOSE) 
	{
		PostQuitMessage(0);
	}
	return DefWindowProc(handle, msg, wparam, lparam);
}

Window::Window(int width, int height) 
{
	w = width;
	h = height;
	// Define window style
	WNDCLASS wc = { 0 };
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WinProc;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = "GenericWindow";
	RegisterClass(&wc);

	// Create the window
	m_pHandle = CreateWindow("GenericWindow", "A Game. Hopefully.",
		WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
		100, 100, width, height,
		nullptr, nullptr, nullptr, nullptr);

}

// Get handle (pretty redundant)
HWND Window::getHandle()
{
	return m_pHandle;
}
