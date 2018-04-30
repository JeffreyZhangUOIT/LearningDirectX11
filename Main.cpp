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
along with ProjectFiasco.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <windows.h>
#include "Renderer.h"
#include "Player.h"
#include "Cursor.h"
#include "GameView.h"
#include "Timer.h"
#include "BoxFactory.h"
#include "GameLogic.h"
//#include "AIPlayer.h"
#include "Bullet.h"
#include "SoundManager.h"
#include "TextHandler.h"
#include "Settings.h"
#include <tchar.h>
#include <Dbt.h>

int* convertFloatToLogicalUnits(float* aBox, int width, int height) {
	int* intArray = (int*)malloc(sizeof(int) * 4);
	intArray[0] = ((aBox[0] + 1.0f) / 2) * width;
	intArray[1] = (((aBox[1] + 1.0f) / 2) * height) + height/1.6;
	intArray[2] = ((aBox[2] + 1.0f) / 2) * width;
	intArray[3] = (((aBox[3] + 1.0f) / 2) * height) + height / 2;
	return intArray;
}

int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount) {
	int screenWidth = 1920, screenHeight = 1080;
	Window window(screenWidth, screenHeight);
	Renderer ren(window);
	GameView playerView(0);
	Player player(ren);
	Mouse cursor(ren);
	BoxFactory dBox1(ren), dBox2(ren), dBox3(ren), dBox4(ren);
	HDC hdc = GetDC(window.getHandle());
	GameLogic gameLogic(ren);
	MSG msg = { 0 };
	Timer time;
	TextHandler text(ren, screenWidth, screenHeight);
	SoundManager music(0.5f);
	Settings userSettings(music);

	HDEVNOTIFY hNewAudio = nullptr;
	// Listen for new audio devices
	DEV_BROADCAST_DEVICEINTERFACE filter = {};
	filter.dbcc_size = sizeof(filter);
	filter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	filter.dbcc_classguid = KSCATEGORY_AUDIO;

	hNewAudio = RegisterDeviceNotification(window.getHandle(), &filter,
		DEVICE_NOTIFY_WINDOW_HANDLE);
	
	time.resetTime();

	int gamemode = 1;
	//SetCapture(window.getHandle());
	int xPosRelative = 0, yPosRelative = 0;
	ShowCursor(false);

	float* startGameBox = (float*) malloc(sizeof(float) * 4);
	startGameBox[0] = -0.8f; startGameBox[1] = -0.7f; startGameBox[2] = -0.5f; startGameBox[3] = -0.5f;
	int* textBox1 = convertFloatToLogicalUnits(startGameBox, screenWidth, screenHeight);

	float* centerLeft = (float*)malloc(sizeof(float) * 4);
	centerLeft[0] = -0.4f; centerLeft[1] = -0.7f; centerLeft[2] = -0.1f; centerLeft[3] = -0.5f;
	int* textBox2 = convertFloatToLogicalUnits(centerLeft, screenWidth, screenHeight);

	float* settings = (float*)malloc(sizeof(float) * 4);
	settings[0] = 0; settings[1] = -0.7f; settings[2] = 0.3f; settings[3] = -0.5f;
	int* textBox3 = convertFloatToLogicalUnits(settings, screenWidth, screenHeight);

	float* quitBox = (float*)malloc(sizeof(float) * 4);
	quitBox[0] = 0.4f; quitBox[1] = -0.7f; quitBox[2] = 0.7f; quitBox[3] = -0.5f;
	int* textBox4 = convertFloatToLogicalUnits(quitBox, screenWidth, screenHeight);

	float* volBackgroundBox = (float*)malloc(sizeof(float) * 4);
	volBackgroundBox[0] = 0.0f; volBackgroundBox[1] = 0.55f; volBackgroundBox[2] =0.9f; volBackgroundBox[3] = 0.65f;
	int* textBox5 = convertFloatToLogicalUnits(volBackgroundBox, screenWidth, screenHeight);

	float* volStrokeBox = (float*)malloc(sizeof(float) * 4);
	volStrokeBox[0] = 0.01f; volStrokeBox[1] = 0.56f; volStrokeBox[2] = 0.89f; volStrokeBox[3] = 0.64f;
	int* textBox6 = convertFloatToLogicalUnits(volStrokeBox, screenWidth, screenHeight);

	float* volValueBox = (float*)malloc(sizeof(float) * 4);
	volValueBox[0] = 0.02f; volValueBox[1] = 0.57f; volValueBox[2] = 0.88f; volValueBox[3] = 0.63f;
	int* textBox7 = convertFloatToLogicalUnits(volValueBox, screenWidth, screenHeight);

	DirectX::XMVECTORF32 white = { 1, 1, 1, 1 };
	DirectX::XMVECTORF32 black = { 0, 0, 0, 1 };

	BoxFactory::color boxWhite = { 1, 1, 1, 1 };
	BoxFactory::color boxBlack = { 0, 0, 0, 1 };
	
	music.Reset();

	while (true) 
	{
		
		unsigned long deltaT = time.elapsedTime();
		playerView.update(screenWidth, screenHeight, xPosRelative, yPosRelative);
		//SetCursorPos(screenWidth / 2, screenHeight / 2);
		

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			bool fHandled = false;
			if (msg.message == WM_LBUTTONDOWN)
			{
				playerView.mouseDown(&msg);
				
				fHandled = true;
			}

			if (msg.message == WM_QUIT)
			{
				break;
			}

			if (msg.message == WM_DEVICECHANGE) {
				if (msg.wParam == DBT_DEVICEARRIVAL)
				{
					auto pDev = reinterpret_cast<PDEV_BROADCAST_HDR>(msg.lParam);
					if (pDev)
					{
						if (pDev->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
						{
							auto pInter = reinterpret_cast<
								const PDEV_BROADCAST_DEVICEINTERFACE>(pDev);

							//GUID KSCATEGORY_AUDIO = { 0x6994AD04, 0x93EF, 0x11D0,{ 0xA3, 0xCC, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96 } };
							if (pInter->dbcc_classguid == KSCATEGORY_AUDIO)
							{
								music.Reset();
							}
						}
					}
				}
			}
			if (!fHandled)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (hNewAudio)
				UnregisterDeviceNotification(hNewAudio);
			
		}
		// ... Main loop ...
		music.Update(gamemode);

		if (gamemode == 0) 
		{
			ren.beginFrame();
			gameLogic.spawnBullet(playerView.getMouse(), playerView.getPos(), playerView.getAim(), ren);
			gameLogic.updateTDS(text, ren, playerView.getPos());

			cursor.update(ren, playerView.getAim());
			cursor.draw(ren);

			player.update(ren, playerView.getPos(), gameLogic.playerDamage());
			
			if (!player.alive) {
				gamemode = 1;
			}
		
			player.draw(ren);
			text.update();
			ren.endFrame();
			
		}


		if (gamemode == 1) 
		{
			
			// Update
			cursor.update(ren, playerView.getAim());
			float* point = playerView.getAim();
			
			if (point[0] > startGameBox[0] && point[1] > startGameBox[1] && point[0] < startGameBox[2] && point[1] < startGameBox[3] && playerView.getMouse()) { // check if cursor is in the damn box. + click.
				gameLogic.initTDS(ren);
				//bot.init(false, false);
				playerView.init();
				player.init();
				gamemode = 0;
			}
			if (point[0] > settings[0] && point[1] > settings[1] && point[0] < settings[2] && point[1] < settings[3] && playerView.getMouse()) { // check if cursor is in the damn box. + click.
				gamemode = 2;
			}
			if (point[0] > quitBox[0] && point[1] > quitBox[1] && point[0] < quitBox[2] && point[1] < quitBox[3] && playerView.getMouse()) { // check if cursor is in the damn box. + click.
				break;
			}

			// Draw
			ren.beginFrame();
			
			dBox1.update(ren, startGameBox, boxWhite);
			dBox1.draw(ren);
			dBox3.update(ren, settings, boxWhite);
			dBox3.draw(ren);
			dBox4.update(ren, quitBox, boxWhite);
			dBox4.draw(ren);
			
			cursor.draw(ren);

			text.drawMyText(L"Just Survive.", screenWidth / 2, screenHeight / 2, white);
			text.drawMyText(L"WASD to move. Hold down left click to shoot.", screenWidth / 2, (screenHeight / 2) + 80 , white);
			text.drawMyText(L"Continue", textBox1[0] + 135, textBox1[1]+30, black);
			text.drawMyText(L"Settings", textBox3[0] + 135, textBox3[1] + 30, black);
			text.drawMyText(L"Quit", textBox4[0] + 135, textBox4[1] + 30, black);
			
			ren.endFrame();
			
		}	

		if (gamemode == 2)
		{
			cursor.update(ren, playerView.getAim());
			float* point = playerView.getAim();

			if (point[0] > volStrokeBox[0] && point[1] > volStrokeBox[1] && point[0] 
				< volStrokeBox[2] && point[1] < volStrokeBox[3] && ((GetKeyState(VK_LBUTTON) & 0x100) != 0)) { // check if cursor is in the damn box. + click.
				float vol = point[0] - 0.02 ;
				vol = vol / 0.86;
				music.setVolume(vol);
				userSettings.update(music, vol);
			}

			if (point[0] > quitBox[0] && point[1] > quitBox[1] && point[0] < quitBox[2] && point[1] < quitBox[3] && playerView.getMouse()) { // check if cursor is in the damn box. + click.
				userSettings.save(music);
				gamemode = 1;
			}

			float masterVolPercentage = music.changeVolume(0);
			volValueBox[2] = (0.86 * masterVolPercentage) + 0.02;

			ren.beginFrame();

			dBox1.update(ren, volBackgroundBox, boxWhite);
			dBox1.draw(ren);
			dBox2.update(ren, volStrokeBox, boxBlack);
			dBox2.draw(ren);
			dBox3.update(ren, volValueBox, boxWhite);
			dBox3.draw(ren);
			dBox4.update(ren, quitBox, boxWhite);
			dBox4.draw(ren);
			cursor.draw(ren);

			text.drawMyText(L"Settings Menu", screenWidth / 2, screenHeight/10, white);
			text.drawMyText(L"Master Volume: ", screenWidth / 3, (screenHeight / 10) * 2, white);
			text.drawMyText(L"Main Menu", textBox4[0] + 135, textBox4[1] + 30, black);


			ren.endFrame();

		}
		
	}
	//ReleaseCapture();
	return 0;
}


