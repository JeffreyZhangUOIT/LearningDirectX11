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
#pragma once

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
#include "SaveManager.h"
#include "Settings.h"
#include "TextGrabber.h"
#include <tchar.h>
#include <Dbt.h>

/*
int* convertFloatToLogicalUnits(float* aBox, int width, int height) {

int* intArray = (int*)malloc(sizeof(int) * 4);
intArray[0] = ((aBox[0] + 1.0f) * width) / 2;
intArray[1] = height - (((aBox[1] + 1) * height) / 2);
intArray[2] = ((aBox[2] + 1.0f) * width) / 2;
intArray[3] = height - (((aBox[3] + 1) * height) / 2);
return intArray;
}


int* centerOfBoxInLU(float* aBox, int width, int height) {

int* intArray = (int*)malloc(sizeof(int) * 4);
intArray[0] = (( ((aBox[0] + aBox[2]) /2) + 1.0f) * width) / 2;
intArray[1] = height - (((((aBox[1] + aBox[3])/2) + 1.0f) * height) / 2);
return intArray;
}
*/


int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount) {
	int screenWidth = 1920, screenHeight = 1080, minigame = 0;
	Window window(screenWidth, screenHeight);
	Renderer ren(window);
	GameView playerView(0);
	Player player(ren);
	Mouse cursor(ren);
	BoxFactory dBox1(ren), dBox2(ren), dBox3(ren), dBox4(ren), dBox5(ren);
	HDC hdc = GetDC(window.getHandle());
	GameLogic gameLogic(ren);
	MSG msg = { 0 };
	Timer time;
	TextHandler text(ren, screenWidth, screenHeight);
	SoundManager music(0.5f);
	TextGrabber storyReader(1);
	HDEVNOTIFY hNewAudio = nullptr;
	Settings userSettings(music, text);
	SaveManager savefile(1);
	// Listen for new audio devices
	DEV_BROADCAST_DEVICEINTERFACE filter = {};
	filter.dbcc_size = sizeof(filter);
	filter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	filter.dbcc_classguid = KSCATEGORY_AUDIO;

	hNewAudio = RegisterDeviceNotification(window.getHandle(), &filter,
		DEVICE_NOTIFY_WINDOW_HANDLE);

	int gamemode = 0;
	int prevGamemode = 0;
	//SetCapture(window.getHandle());
	int xPosRelative = 0, yPosRelative = 0;
	ShowCursor(false);

	// Seriously consider making a menu system.
	float* startGameBox = (float*) malloc(sizeof(float) * 4);
	startGameBox[0] = -0.8f; startGameBox[1] = -0.7f; startGameBox[2] = -0.5f; startGameBox[3] = -0.5f;

	float* continueBox = (float*)malloc(sizeof(float) * 4);
	continueBox[0] = -0.4f; continueBox[1] = -0.7f; continueBox[2] = -0.1f; continueBox[3] = -0.5f;
	//int* textBox2 = convertFloatToLogicalUnits(centerLeft, screenWidth, screenHeight);

	float* settings = (float*)malloc(sizeof(float) * 4);
	settings[0] = 0; settings[1] = -0.7f; settings[2] = 0.3f; settings[3] = -0.5f;

	float* quitBox = (float*)malloc(sizeof(float) * 4);
	quitBox[0] = 0.4f; quitBox[1] = -0.7f; quitBox[2] = 0.7f; quitBox[3] = -0.5f;
	

	float* dialogBox = (float*)malloc(sizeof(float) * 4);
	dialogBox[0] = -.9f; dialogBox[1] = -0.9f; dialogBox[2] = 0.9f; dialogBox[3] = -0.35f;

	// Volume control Boxes
	float* volBackgroundBox = (float*)malloc(sizeof(float) * 4);
	volBackgroundBox[0] = 0.0f; volBackgroundBox[1] = 0.55f; volBackgroundBox[2] =0.9f; volBackgroundBox[3] = 0.65f;

	float* volStrokeBox = (float*)malloc(sizeof(float) * 4);
	volStrokeBox[0] = 0.01f; volStrokeBox[1] = 0.56f; volStrokeBox[2] = 0.89f; volStrokeBox[3] = 0.64f;

	float* volValueBox = (float*)malloc(sizeof(float) * 4);
	volValueBox[0] = 0.02f; volValueBox[1] = 0.57f; volValueBox[2] = 0.88f; volValueBox[3] = 0.63f;

	// Speed Scroll Boxes
	float* speedBackgroundBox = (float*)malloc(sizeof(float) * 4);
	speedBackgroundBox[0] = 0.0f; speedBackgroundBox[1] = 0.35f; speedBackgroundBox[2] = 0.9f; speedBackgroundBox[3] = 0.45f;

	float* speedStrokBox = (float*)malloc(sizeof(float) * 4);
	speedStrokBox[0] = 0.01f; speedStrokBox[1] = 0.36f; speedStrokBox[2] = 0.89f; speedStrokBox[3] = 0.44f;

	float* speedValueBox = (float*)malloc(sizeof(float) * 4);
	speedValueBox[0] = 0.02f; speedValueBox[1] = 0.37f; speedValueBox[2] = 0.88f; speedValueBox[3] = 0.43f;

	// Light Novel Boxes
	float* saveBox = (float*)malloc(sizeof(float) * 4);
	saveBox[0] = 0.1f; saveBox[1] = -0.35f; saveBox[2] = 0.25f; saveBox[3] = -0.25f;

	float* loadBox = (float*)malloc(sizeof(float) * 4);
	loadBox[0] = 0.30f; loadBox[1] = -0.35f; loadBox[2] = 0.45f; loadBox[3] = -0.25f;
	
	float* optionsBox = (float*)malloc(sizeof(float) * 4);
	optionsBox[0] = 0.5f; optionsBox[1] = -0.35f; optionsBox[2] = 0.65f; optionsBox[3] = -0.25f;

	float* quitToMain = (float*)malloc(sizeof(float) * 4);
	quitToMain[0] = 0.7f; quitToMain[1] = -0.35f; quitToMain[2] = 0.85f; quitToMain[3] = -0.25f;
	
	DirectX::XMVECTORF32 white = { 1, 1, 1, 1 };
	DirectX::XMVECTORF32 black = { 0, 0, 0, 1 };
	DirectX::XMVECTORF32 red = { 1, 0, 0, 1 };

	BoxFactory::color boxWhite = { 1, 1, 1, 1 };
	BoxFactory::color boxBlack = { 0, 0, 0, 1 };
	
	music.Reset();
	time.resetTime();

	while (true) 
	{
		playerView.update(screenWidth, screenHeight, xPosRelative, yPosRelative, time);
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
			
			dBox1.update(ren, startGameBox, boxWhite);
			dBox1.draw(ren);
			dBox3.update(ren, settings, boxWhite);
			dBox3.draw(ren);
			dBox4.update(ren, quitBox, boxWhite);
			dBox4.draw(ren);
			dBox4.update(ren, continueBox, boxWhite);
			dBox4.draw(ren);

			cursor.draw(ren);

			// Main menu
			cursor.update(ren, playerView.getAim());
			float* point = playerView.getAim();

			// Start Game box
			if (point[0] > startGameBox[0] && point[1] > startGameBox[1] && point[0] < startGameBox[2] && point[1] < startGameBox[3] && playerView.getMouse()) {
				storyReader.setChOff(1, 0);
				gamemode = 2;
			}

			// Continue box
			if (point[0] > continueBox[0] && point[1] > continueBox[1] && point[0] < continueBox[2] && point[1] < continueBox[3] && playerView.getMouse()) {
				gamemode = 2;
				text.tempMessage(0, 0.9f, L"Loaded.", red);
				savefile.Load(storyReader, text);
				
			}
			// Settings Box
			if (point[0] > settings[0] && point[1] > settings[1] && point[0] < settings[2] && point[1] < settings[3] && playerView.getMouse()) {
				prevGamemode = gamemode;
				gamemode = 1;
			}

			// Quit Box
			if (point[0] > quitBox[0] && point[1] > quitBox[1] && point[0] < quitBox[2] && point[1] < quitBox[3] && playerView.getMouse()) {
				break;
			}

			text.drawMyText(L"Knockoff Light Novel.", 0, 0, white);
			text.drawMyText(L"WASD to move. Hold down left click to shoot.", 0, -0.1f, white);
			text.drawMyText(L"New Game", (startGameBox[0] + startGameBox[2]) /2, (startGameBox[1] + startGameBox[3])/2, black);
			text.drawMyText(L"Continue", (continueBox[0] + continueBox[2]) / 2, (continueBox[1] + continueBox[3]) / 2, black);
			text.drawMyText(L"Settings", (settings[0] + settings[2])/2, (settings[1] + settings[3])/2, black);
			text.drawMyText(L"Quit", (quitBox[0] + quitBox[2]) / 2, (quitBox[1] + quitBox[3]) / 2, black);
			
			ren.endFrame();
			
		}	

		if (gamemode == 1)
		{
			// Settings menu
			cursor.update(ren, playerView.getAim());
			float* point = playerView.getAim();

			// Volume Box
			if (point[0] > volStrokeBox[0] && point[1] > volStrokeBox[1] && point[0] 
				< volStrokeBox[2] && point[1] < volStrokeBox[3] && ((GetKeyState(VK_LBUTTON) & 0x100) != 0)) { 
				float vol = point[0] - 0.02 ;
				vol = vol / 0.86;
				userSettings.updateMasterVol(music, vol);
			}

			// Text Scroll Speed Box
			if (point[0] > speedStrokBox[0] && point[1] > speedStrokBox[1] && point[0]
				< speedStrokBox[2] && point[1] < speedStrokBox[3] && ((GetKeyState(VK_LBUTTON) & 0x100) != 0)) {
				double speed = point[0] - 0.02;
				speed = speed / 0.86;
				speed = (400 * speed) + 10;
				userSettings.updateCharPerSec(text, speed);
			}

			// Return box.
			if (point[0] > quitBox[0] && point[1] > quitBox[1] && point[0] < quitBox[2] && point[1] < quitBox[3] && playerView.getMouse()) {
				userSettings.save(music);
				gamemode = prevGamemode;
				storyReader.offset -= 1;
			}

			float masterVolPercentage = music.changeVolume(0);
			double charPerSecPercentage = text.charPerSec;
			volValueBox[2] = (0.86 * masterVolPercentage) + 0.02;
			speedValueBox[2] = (0.86 * ((text.charPerSec - 10)/400)) + 0.02;
			ren.beginFrame();

			dBox1.update(ren, volBackgroundBox, boxWhite);
			dBox1.draw(ren);
			dBox2.update(ren, volStrokeBox, boxBlack);
			dBox2.draw(ren);
			dBox3.update(ren, volValueBox, boxWhite);
			dBox3.draw(ren);
			
			dBox1.update(ren, speedBackgroundBox, boxWhite);
			dBox1.draw(ren);
			dBox2.update(ren, speedStrokBox, boxBlack);
			dBox2.draw(ren);
			dBox3.update(ren, speedValueBox, boxWhite);
			dBox3.draw(ren);

			dBox4.update(ren, quitBox, boxWhite);
			dBox4.draw(ren);
			cursor.draw(ren);

			text.drawMyText(L"Settings Menu", 0, 0.9f, white);
			text.drawMyText(L"Master Volume: ", -0.5f, (volBackgroundBox[1] + volBackgroundBox[3]) / 2, white);
			text.drawMyText(L"Text Scroll Speed: ", -0.5f, (speedBackgroundBox[1] + speedBackgroundBox[3]) / 2, white);
			text.drawMyText(L"Return", (quitBox[0] + quitBox[2]) /2 , (quitBox[1] + quitBox[3])/2, black);


			ren.endFrame();
		}

		if (gamemode == 2)
		{
			// Light novel portion.
			cursor.update(ren, playerView.getAim());
			float* point = playerView.getAim();

			ren.beginFrame();

			dBox1.update(ren, dialogBox, boxWhite);
			dBox1.draw(ren);
			dBox1.update(ren, optionsBox, boxWhite);
			dBox1.draw(ren);
			dBox1.update(ren, saveBox, boxWhite);
			dBox1.draw(ren);
			dBox1.update(ren, loadBox, boxWhite);
			dBox1.draw(ren);
			dBox1.update(ren, quitToMain, boxWhite);
			dBox1.draw(ren);
		
			//cursor.update(ren, playerView.getAim());
			cursor.draw(ren);

			gamemode = storyReader.execCommand(music, text);

			if (point[0] > quitToMain[0] && point[1] > quitToMain[1] && point[0] < quitToMain[2] && point[1] < quitToMain[3] && playerView.getMouse()) {
				gamemode = 0;

			}
			if (point[0] > dialogBox[0] && point[1] > dialogBox[1] && point[0] < dialogBox[2] && point[1] < dialogBox[3] && playerView.getMouse()) {
				storyReader.getNextCommand();
			}

			if (point[0] > optionsBox[0] && point[1] > optionsBox[1] && point[0] < optionsBox[2] && point[1] < optionsBox[3] && playerView.getMouse()) {
				prevGamemode = gamemode;
				gamemode = 1;
			}

			if (point[0] > saveBox[0] && point[1] > saveBox[1] && point[0] < saveBox[2] && point[1] < saveBox[3] && playerView.getMouse()) {
				text.tempMessage(0, 0.9f, L"Saved.", red);
				savefile.Save(storyReader, text);
			}

			if (point[0] > loadBox[0] && point[1] > loadBox[1] && point[0] < loadBox[2] && point[1] < loadBox[3] && playerView.getMouse()) {
				text.tempMessage(0, 0.9f, L"Loaded.", red);
				savefile.Load(storyReader, text);
				storyReader.getNextCommand();
			}
			
			text.drawMyText(L"Save", (saveBox[0] + saveBox[2])/2, ((saveBox[1], saveBox[3]) / 2) - 0.18f, red);
			text.drawMyText(L"Load", (loadBox[0] + loadBox[2]) / 2, ((saveBox[1], saveBox[3]) / 2) - 0.18f, red);
			text.drawMyText(L"Conf", (optionsBox[0] + optionsBox[2]) / 2, ((optionsBox[1], optionsBox[3]) / 2) - 0.18f, red);
			text.drawMyText(L"Quit", (quitToMain[0] + quitToMain[2]) / 2, ((saveBox[1], saveBox[3]) / 2) - 0.18f, red);
			text.update();

			ren.endFrame();
			if (gamemode == 3) {
				gameLogic.initTDS(ren, time);
				//bot.init(false, false);
				playerView.init();
				player.init();
			}
		}

		if (gamemode == 3)
		{
			// Top-down shooter minigame.
			ren.beginFrame();
			gameLogic.spawnBullet(playerView.getMouse(), playerView.getPos(), playerView.getAim(), ren, time);
			gameLogic.updateTDS(text, ren, playerView.getPos(), time);

			cursor.update(ren, playerView.getAim());
			cursor.draw(ren);

			player.update(ren, playerView.getPos(), gameLogic.playerDamage());

			// Player died, determine which scene to go to and go.
			if (!player.alive) {
				char nextScene[256] = "SCENE2B";
				storyReader.minigameReturn(nextScene);
				gamemode = 2;
			}

			player.draw(ren);
			text.update();
			ren.endFrame();

		}
		
	}
	//ReleaseCapture();
	return 0;
}


