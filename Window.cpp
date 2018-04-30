/*
Copyright 2017, 2018 Jeffrey Zhang

This file is part of ProjectFiasco.

ProjectFiasco is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ProjectFiasco is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/
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
