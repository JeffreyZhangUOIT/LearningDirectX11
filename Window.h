#pragma once
#include <Windows.h>

class Window {
public:
	Window(int width, int height);
	int w;
	int h;
	HWND getHandle();
private:
	HWND m_pHandle;
	
};