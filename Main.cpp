#include <Windows.h>
#include "Window.h"
#include "Renderer.h"
#include "Player.h"
#include "Cursor.h"
#include "GameObject.h"
#include "GameView.h"
#include "Text.h"
#include "Dialog.h"
#include "GameLogic.h"
#include <tchar.h>

int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount) {
	Window window(800, 600);
	Renderer ren(window);
	GameView playerView(0);
	Player player(ren);
	Mouse cursor(ren);
	Dialog dBox(ren);
	HDC hdc = GetDC(window.getHandle());
	GameLogic gameLogic(ren);
	MSG msg = { 0 };
	Text myText(ren);
	int gamemode = 0;

	int screenWidth = GetDeviceCaps(hdc, HORZRES);
	int screenHeight = GetDeviceCaps(hdc, VERTRES);

	while (true) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
			if (msg.message == WM_QUIT) break;
		}

		// ... Main loop ...
		if (gamemode == 0) 
		{
			
			playerView.changeAim(screenWidth, screenHeight);
			playerView.changePos();
			ren.beginFrame();

			gameLogic.updateTDS(ren, playerView.getPos());
			if (gameLogic.gameOver()) {
				gamemode = 1;
			}
			gameLogic.drawTDS(ren);

			cursor.update(ren, playerView.getAim());
			cursor.draw(ren);

			player.update(ren, playerView.getPos());
			player.draw(ren);

			ren.endFrame();
		}

		if (gamemode == 1) 
		{
			// Update
			playerView.changeAim(screenWidth, screenHeight);
			cursor.update(ren, playerView.getAim());
			float* point = playerView.getAim();
			if (point[0] < -0.9f && point[1] < -0.9f) {
				gameLogic.initTDS(ren);
				playerView.init();
				gamemode = 0;
			}

			// Draw
			ren.beginFrame();
			dBox.update(ren);
			dBox.draw(ren);
			cursor.draw(ren);
			ren.endFrame();
			
			// The text doesn't work yet, need to get them to render to the same surface.
			//const wchar_t* string = L"Some Testing Text";
			//myText.DrawMyText(50.f, 400.f, 750.f, 550.f, string, 0, ren);


		}
		
	}
	
	return 0;
}
