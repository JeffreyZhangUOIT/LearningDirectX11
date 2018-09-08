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

The MIT License (MIT)

Copyright (c) 2018 Microsoft Corp

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <windows.h>
#include "Renderer.h"
#include "Cursor.h"
#include "GameView.h"
#include "Timer.h"
#include "BoxFactory.h"
#include "GameLogic.h"
//#include "AIPlayer.h"
#include "SoundManager.h"
#include "TextHandler.h"
#include "MinigameManager.h"
#include "PoseArtManager.h"
#include "SaveManager.h"
#include "Settings.h"
#include "TextureManager.h"
#include "BackgroundManager.h"
#include "TextGrabber.h"
#include "Choice.h"
#include "TransitionManager.h"
#include <tchar.h>



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
	Settings userSettings(0);
	// 1366x768 1920x1080 1280x800 1440x900 800x600 1280x1024 1600x900 3840x2160 1024x768
	int screenWidth = 1280, screenHeight = 720, minigame = 0;
	if (strncmp(userSettings.resolution, "1366x768", strlen("1366x768")) == 0) {
		screenWidth = 1366;
		screenHeight = 768;
	}
	if (strncmp(userSettings.resolution, "1024x768", strlen("1024x768")) == 0) {
		screenWidth = 1024;
		screenHeight = 768;
	}
	if (strncmp(userSettings.resolution, "1920x1080", strlen("1366x768")) == 0) {
		screenWidth = 1920;
		screenHeight = 1080;
	}
	if (strncmp(userSettings.resolution, "1280x800", strlen("1280x800")) == 0) {
		screenWidth = 1280;
		screenHeight = 800;
	}
	if (strncmp(userSettings.resolution, "1440x900", strlen("1440x900")) == 0) {
		screenWidth = 1440;
		screenHeight = 900;
	}
	if (strncmp(userSettings.resolution, "800x600", strlen("800x600")) == 0) {
		screenWidth = 800;
		screenHeight = 600;
	}
	if (strncmp(userSettings.resolution, "1280x1024", strlen("1280x1024")) == 0) {
		screenWidth = 1280;
		screenHeight = 1024;
	}
	if (strncmp(userSettings.resolution, "1600x900", strlen("1600x900")) == 0) {
		screenWidth = 1600;
		screenHeight = 900;
	}
	if (strncmp(userSettings.resolution, "3840x2160", strlen("3840x2160")) == 0) {
		screenWidth = 3840;
		screenHeight = 2160;
	}
	Window window(screenWidth, screenHeight);
	Renderer ren(window);
	TextureManager tex(ren);
	GameView playerView(0);
	Mouse cursor(ren);
	BoxFactory dBox1(ren), dBox2(ren), dBox3(ren), dBox4(ren), dBox5(ren);
	GameLogic gameLogic(ren);
	MSG msg = { 0 };
	Timer time;
	TextHandler text(ren, screenWidth, screenHeight);
	SoundManager music(0.5f);
	TextGrabber storyReader(1);
	MinigameManager miniMan(false);
	HDEVNOTIFY hNewAudio = nullptr;
	Choice choice(ren);
	PoseArtManager pose(ren);
	SaveManager savefile(1);
	Transition transistor(ren);
	Background backdrop(ren);

	
	// Listen for new audio devices
	DEV_BROADCAST_DEVICEINTERFACE filter = {};
	filter.dbcc_size = sizeof(filter);
	filter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	filter.dbcc_classguid = KSCATEGORY_AUDIO;

	hNewAudio = RegisterDeviceNotification(window.getHandle(), &filter,
		DEVICE_NOTIFY_WINDOW_HANDLE);

	bool showTextBox = true;
	bool showResOptions = false;
	int delay = 0;
	int gamemode = 0;
	int prevGamemode = 0;
	//SetCapture(window.getHandle());
	int xPosRelative = 0, yPosRelative = 0;
	ShowCursor(false);
	userSettings.init(music, text);

	// Seriously consider making a menu system.
	float* startGameBox = (float*) malloc(sizeof(float) * 4);
	startGameBox[0] = -0.8f; startGameBox[1] = -0.7f; startGameBox[2] = -0.5f; startGameBox[3] = -0.5f;

	float* continueBox = (float*)malloc(sizeof(float) * 4);
	continueBox[0] = -0.4f; continueBox[1] = -0.7f; continueBox[2] = -0.1f; continueBox[3] = -0.5f;

	float* settings = (float*)malloc(sizeof(float) * 4);
	settings[0] = 0; settings[1] = -0.7f; settings[2] = 0.3f; settings[3] = -0.5f;

	float* quitBox = (float*)malloc(sizeof(float) * 4);
	quitBox[0] = 0.4f; quitBox[1] = -0.7f; quitBox[2] = 0.7f; quitBox[3] = -0.5f;

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

	// Resolution Boxes
	float* resolutionSelected = (float*)malloc(sizeof(float) * 4);
	resolutionSelected[0] = 0.0f; resolutionSelected[1] = 0.15f; resolutionSelected[2] = 0.9f; resolutionSelected[3] = 0.25f;
	
	// 800x600
	float* resOp0 = (float*)malloc(sizeof(float) * 4);
	resOp0[0] = 0.0f; resOp0[1] = 0.05f; resOp0[2] = 0.9f; resOp0[3] = 0.153f;

	// 1024x768
	float* resOp1 = (float*)malloc(sizeof(float) * 4);
	resOp1[0] = 0.0f; resOp1[1] = -0.05f; resOp1[2] = 0.9f; resOp1[3] = 0.053f;

	// 1280x800
	float* resOp2 = (float*)malloc(sizeof(float) * 4);
	resOp2[0] = 0.0f; resOp2[1] = -0.15f; resOp2[2] = 0.9f; resOp2[3] = -0.053f;

	// 1280x1024
	float* resOp3 = (float*)malloc(sizeof(float) * 4);
	resOp3[0] = 0.0f; resOp3[1] = -0.25f; resOp3[2] = 0.9f; resOp3[3] = -0.153f;

	// 1366x768
	float* resOp4 = (float*)malloc(sizeof(float) * 4);
	resOp4[0] = 0.0f; resOp4[1] = -0.35f; resOp4[2] = 0.9f; resOp4[3] = -0.253f;

	// 1440x900
	float* resOp5 = (float*)malloc(sizeof(float) * 4);
	resOp5[0] = 0.0f; resOp5[1] = -0.45f; resOp5[2] = 0.9f; resOp5[3] = -0.353f;

	// 1600x900
	float* resOp6 = (float*)malloc(sizeof(float) * 4);
	resOp6[0] = 0.0f; resOp6[1] = -0.55f; resOp6[2] = 0.9f; resOp6[3] = -0.453f;

	// 1920x1080
	float* resOp7 = (float*)malloc(sizeof(float) * 4);
	resOp7[0] = 0.0f; resOp7[1] = -0.65f; resOp7[2] = 0.9f; resOp7[3] = -0.553f;

	// 3840x2160
	float* resOp8 = (float*)malloc(sizeof(float) * 4);
	resOp8[0] = 0.0f; resOp8[1] = -0.75f; resOp8[2] = 0.9f; resOp8[3] = -0.653f;
	
	// Apply Resolution
	float* applyRes = (float*)malloc(sizeof(float) * 4);
	applyRes[0] = -0.7f; applyRes[1] = -0.7f; applyRes[2] = -0.4f; applyRes[3] = -0.5f;

	// Light Novel Boxes
	float* dialogBox = (float*)malloc(sizeof(float) * 4);
	dialogBox[0] = -1.0f; dialogBox[1] = -1.0f; dialogBox[2] = 1.0f; dialogBox[3] = -0.3f;

	float* saveBox = (float*)malloc(sizeof(float) * 4);
	saveBox[0] = 0.1f; saveBox[1] = -0.3f; saveBox[2] = 0.25f; saveBox[3] = -0.2f;

	float* loadBox = (float*)malloc(sizeof(float) * 4);
	loadBox[0] = 0.30f; loadBox[1] = -0.3f; loadBox[2] = 0.45f; loadBox[3] = -0.2f;

	float* optionsBox = (float*)malloc(sizeof(float) * 4);
	optionsBox[0] = 0.5f; optionsBox[1] = -0.3f; optionsBox[2] = 0.65f; optionsBox[3] = -0.2f;

	float* quitToMain = (float*)malloc(sizeof(float) * 4);
	quitToMain[0] = 0.7f; quitToMain[1] = -0.3f; quitToMain[2] = 0.85f; quitToMain[3] = -0.2f;
	
	
	DirectX::XMVECTORF32 white = { 1, 1, 1, 1 };
	DirectX::XMVECTORF32 black = { 0, 0, 0, 1 };
	DirectX::XMVECTORF32 red = { 1, 0, 0, 1 };
	
	music.Reset();
	time.resetTime();

	// ... Main loop ...
	while (true) 
	{
		playerView.update(screenWidth, screenHeight, xPosRelative, yPosRelative, time, window.getHandle());
		
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
			{
				UnregisterDeviceNotification(hNewAudio);
			}
		}
		
		music.Update(gamemode);	

		// Main menu
		if (gamemode == 0) 
		{
			float* point = playerView.getAim();

			ren.beginFrame();
			backdrop.setBackground(6);
			backdrop.draw(ren, tex);

			dBox1.update(ren, startGameBox, 15, point[0], point[1]);
			dBox1.draw(ren, tex);
			dBox2.update(ren, quitBox, 15, point[0], point[1]);
			dBox2.draw(ren, tex);
			dBox3.update(ren, settings, 15, point[0], point[1]);
			dBox3.draw(ren, tex);
			dBox4.update(ren, continueBox, 15, point[0], point[1]);
			dBox4.draw(ren, tex);

			// Start Game box
			if (point[0] > startGameBox[0] && point[1] > startGameBox[1] && point[0] < startGameBox[2] && point[1] < startGameBox[3] && playerView.getMouse()) {
				storyReader.setChOff(1, 0);
				pose.exitAll();
				gamemode = 2;
			}
			// Continue box
			if (point[0] > continueBox[0] && point[1] > continueBox[1] && point[0] < continueBox[2] && point[1] < continueBox[3] && playerView.getMouse()) {
				gamemode = 2;
				text.tempMessage(50, 0, 0.9f, L"Loaded.", red);
				pose.exitAll();
				savefile.Load(storyReader, text, pose, transistor, time);
				
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

			text.drawMyText(L"New Game", (startGameBox[0] + startGameBox[2]) /2, (startGameBox[1] + startGameBox[3])/2, black);
			text.drawMyText(L"Continue", (continueBox[0] + continueBox[2]) / 2, (continueBox[1] + continueBox[3]) / 2, black);
			text.drawMyText(L"Settings", (settings[0] + settings[2])/2, (settings[1] + settings[3])/2, black);
			text.drawMyText(L"Quit", (quitBox[0] + quitBox[2]) / 2, (quitBox[1] + quitBox[3]) / 2, black);

			ren.resetFrame();
			cursor.update(ren, playerView.getAim());
			cursor.draw(ren, tex);

			ren.endFrame();
		}	

		if (gamemode == 1)
		{
			// Settings menu
			float* point = playerView.getAim();

			// Volume Box
			if (point[0] > volStrokeBox[0] && point[1] > volStrokeBox[1] && point[0] 
				< volStrokeBox[2] && point[1] < volStrokeBox[3] && ((GetKeyState(VK_LBUTTON) & 0x100) != 0)) 
			{
				float vol = point[0] - 0.02 ;
				vol = vol / 0.86;
				userSettings.updateMasterVol(music, vol);
			}

			// Text Scroll Speed Box
			if (point[0] > speedStrokBox[0] && point[1] > speedStrokBox[1] && point[0]
				< speedStrokBox[2] && point[1] < speedStrokBox[3] && ((GetKeyState(VK_LBUTTON) & 0x100) != 0)) 
			{
				double speed = point[0] - 0.02;
				speed = speed / 0.86;
				speed = (400 * speed) + 10;
				userSettings.updateCharPerSec(text, speed);
			}

			// Resolution Box
			if (point[0] > resolutionSelected[0] && point[1] > resolutionSelected[1] && point[0]
				< resolutionSelected[2] && point[1] < resolutionSelected[3] && ((GetKeyState(VK_LBUTTON) & 0x100) != 0))
			{
				showResOptions = true;
			}

			// Resolution Options
			if (showResOptions == true)
			{
				// 800x600 1024x768 1280x800 1280x1024 1366x768 1440x900 1600x900 1920x1080 3840x2160

				if (point[0] > resOp0[0] && point[1] > resOp0[1] && point[0]
					< resOp0[2] && point[1] < resOp0[3] && ((GetKeyState(VK_LBUTTON) & 0x100) != 0))
				{
					strcpy_s(userSettings.resolution, sizeof(userSettings.resolution), "800x600\0");

					showResOptions = false;
				}

				if (point[0] > resOp1[0] && point[1] > resOp1[1] && point[0]
					< resOp1[2] && point[1] < resOp1[3] && ((GetKeyState(VK_LBUTTON) & 0x100) != 0))
				{
					strcpy_s(userSettings.resolution, sizeof(userSettings.resolution), "1024x768\0");

					showResOptions = false;
				}

				if (point[0] > resOp2[0] && point[1] > resOp2[1] && point[0]
					< resOp2[2] && point[1] < resOp2[3] && ((GetKeyState(VK_LBUTTON) & 0x100) != 0))
				{
					strcpy_s(userSettings.resolution, sizeof(userSettings.resolution), "1280x800\0");

					showResOptions = false;
				}

				if (point[0] > resOp3[0] && point[1] > resOp3[1] && point[0]
					< resOp3[2] && point[1] < resOp3[3] && ((GetKeyState(VK_LBUTTON) & 0x100) != 0))
				{
					strcpy_s(userSettings.resolution, sizeof(userSettings.resolution), "1280x1024\0");

					showResOptions = false;
				}

				if (point[0] > resOp4[0] && point[1] > resOp4[1] && point[0]
					< resOp4[2] && point[1] < resOp4[3] && ((GetKeyState(VK_LBUTTON) & 0x100) != 0))
				{
					strcpy_s(userSettings.resolution, sizeof(userSettings.resolution), "1366x768\0");

					showResOptions = false;
				}

				if (point[0] > resOp5[0] && point[1] > resOp5[1] && point[0]
					< resOp5[2] && point[1] < resOp5[3] && ((GetKeyState(VK_LBUTTON) & 0x100) != 0))
				{
					strcpy_s(userSettings.resolution, sizeof(userSettings.resolution), "1440x900\0");

					showResOptions = false;
				}

				if (point[0] > resOp6[0] && point[1] > resOp6[1] && point[0]
					< resOp6[2] && point[1] < resOp6[3] && ((GetKeyState(VK_LBUTTON) & 0x100) != 0))
				{
					strcpy_s(userSettings.resolution, sizeof(userSettings.resolution), "1600x900\0");

					showResOptions = false;
				}

				if (point[0] > resOp7[0] && point[1] > resOp7[1] && point[0]
					< resOp7[2] && point[1] < resOp7[3] && ((GetKeyState(VK_LBUTTON) & 0x100) != 0))
				{
					strcpy_s(userSettings.resolution, sizeof(userSettings.resolution), "1920x1080\0");

					showResOptions = false;
				}

				if (point[0] > resOp8[0] && point[1] > resOp8[1] && point[0]
					< resOp8[2] && point[1] < resOp8[3] && ((GetKeyState(VK_LBUTTON) & 0x100) != 0))
				{
					strcpy_s(userSettings.resolution, sizeof(userSettings.resolution), "3840x2160\0");

					showResOptions = false;
				}
			}

			// Return box.
			if (point[0] > quitBox[0] && point[1] > quitBox[1] && point[0] < quitBox[2] && point[1] < quitBox[3] && playerView.getMouse()) 
			{
				userSettings.save(music);
				gamemode = prevGamemode;
				storyReader.offset -= 1;
			}

			// Apply box.
			if (point[0] > applyRes[0] && point[1] > applyRes[1] && point[0] < applyRes[2] && point[1] < applyRes[3] && playerView.getMouse())
			{
				if (strncmp(userSettings.resolution, "1366x768", strlen("1366x768")) == 0) {
					screenWidth = 1366;
					screenHeight = 768;
				}
				if (strncmp(userSettings.resolution, "1024x768", strlen("1024x768")) == 0) {
					screenWidth = 1024;
					screenHeight = 768;
				}
				if (strncmp(userSettings.resolution, "1920x1080", strlen("1366x768")) == 0) {
					screenWidth = 1920;
					screenHeight = 1080;
				}
				if (strncmp(userSettings.resolution, "1280x800", strlen("1280x800")) == 0) {
					screenWidth = 1280;
					screenHeight = 800;
				}
				if (strncmp(userSettings.resolution, "1440x900", strlen("1440x900")) == 0) {
					screenWidth = 1440;
					screenHeight = 900;
				}
				if (strncmp(userSettings.resolution, "800x600", strlen("800x600")) == 0) {
					screenWidth = 800;
					screenHeight = 600;
				}
				if (strncmp(userSettings.resolution, "1280x1024", strlen("1280x1024")) == 0) {
					screenWidth = 1280;
					screenHeight = 1024;
				}
				if (strncmp(userSettings.resolution, "1600x900", strlen("1600x900")) == 0) {
					screenWidth = 1600;
					screenHeight = 900;
				}
				if (strncmp(userSettings.resolution, "3840x2160", strlen("3840x2160")) == 0) {
					screenWidth = 3840;
					screenHeight = 2160;
				}

				window.ResizeWindow(screenWidth, screenHeight);
				ren.resizeScreen(window);
			}

			float masterVolPercentage = music.changeVolume(0);
			double charPerSecPercentage = text.charPerSec;
			volValueBox[2] = (0.86 * masterVolPercentage) + 0.02;
			speedValueBox[2] = (0.86 * ((text.charPerSec - 10)/400)) + 0.02;
			ren.beginFrame();

			dBox1.update(ren, volBackgroundBox, -1, point[0], point[1]);
			dBox1.draw(ren, tex);
			dBox2.update(ren, volStrokeBox, 4, point[0], point[1]);
			dBox2.draw(ren, tex);
			dBox3.update(ren, volValueBox, -1, point[0], point[1]);
			dBox3.draw(ren, tex);
			
			dBox1.update(ren, speedBackgroundBox, -1, point[0], point[1]);
			dBox1.draw(ren, tex);
			dBox2.update(ren, speedStrokBox, 4, point[0], point[1]);
			dBox2.draw(ren, tex);
			dBox3.update(ren, speedValueBox, -1, point[0], point[1]);
			dBox3.draw(ren, tex);

			dBox5.update(ren, resolutionSelected, -1, point[0], point[1]);
			dBox5.draw(ren, tex);

			dBox4.update(ren, quitBox, 15, point[0], point[1]);
			dBox4.draw(ren, tex);

			dBox1.update(ren, applyRes, -1, point[0], point[1]);
			dBox1.draw(ren, tex);
			
			if (showResOptions == true)
			{
				dBox5.update(ren, resOp0, -1, point[0], point[1]);
				dBox5.draw(ren, tex);
				dBox1.update(ren, resOp1, -1, point[0], point[1]);
				dBox1.draw(ren, tex);
				dBox2.update(ren, resOp2, -1, point[0], point[1]);
				dBox2.draw(ren, tex);
				dBox3.update(ren, resOp3, -1, point[0], point[1]);
				dBox3.draw(ren, tex);
				dBox4.update(ren, resOp4, -1, point[0], point[1]);
				dBox4.draw(ren, tex);

				dBox1.update(ren, resOp5, -1, point[0], point[1]);
				dBox1.draw(ren, tex);
				dBox2.update(ren, resOp6, -1, point[0], point[1]);
				dBox2.draw(ren, tex);
				dBox3.update(ren, resOp7, -1, point[0], point[1]);
				dBox3.draw(ren, tex);
				dBox4.update(ren, resOp8, -1, point[0], point[1]);
				dBox4.draw(ren, tex);

			}
			cursor.update(ren, playerView.getAim());
			cursor.draw(ren, tex);


			wchar_t * buffer;
			int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, userSettings.resolution, -1, NULL, 0);
			buffer = new wchar_t[size];
			if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, userSettings.resolution, -1, buffer, size)) {
				exit(99);
			}

			text.drawMyText(buffer, (resolutionSelected[0] + resolutionSelected[2]) / 2, (resolutionSelected[1] + resolutionSelected[3]) / 2, black);
			text.drawMyText(L"Settings Menu", 0, 0.9f, white);
			text.drawMyText(L"Master Volume: ", -0.5f, (volBackgroundBox[1] + volBackgroundBox[3]) / 2, white);
			text.drawMyText(L"Text Scroll Speed: ", -0.5f, (speedBackgroundBox[1] + speedBackgroundBox[3]) / 2, white);
			text.drawMyText(L"Resolution: ", -0.5f, (resolutionSelected[1] + resolutionSelected[3]) / 2, white);
			text.drawMyText(L"Return", (quitBox[0] + quitBox[2]) /2 , (quitBox[1] + quitBox[3])/2, black);
			text.drawMyText(L"Apply", (applyRes[0] + applyRes[2]) / 2, (applyRes[1] + applyRes[3]) / 2, black);

			ren.resetFrame();

			if (showResOptions == true)
			{
				dBox5.update(ren, resOp0, -1, point[0], point[1]);
				dBox5.draw(ren, tex);
				dBox1.update(ren, resOp1, -1, point[0], point[1]);
				dBox1.draw(ren, tex);
				dBox2.update(ren, resOp2, -1, point[0], point[1]);
				dBox2.draw(ren, tex);
				dBox3.update(ren, resOp3, -1, point[0], point[1]);
				dBox3.draw(ren, tex);
				dBox4.update(ren, resOp4, -1, point[0], point[1]);
				dBox4.draw(ren, tex);

				dBox1.update(ren, resOp5, -1, point[0], point[1]);
				dBox1.draw(ren, tex);
				dBox2.update(ren, resOp6, -1, point[0], point[1]);
				dBox2.draw(ren, tex);
				dBox3.update(ren, resOp7, -1, point[0], point[1]);
				dBox3.draw(ren, tex);
				dBox4.update(ren, resOp8, -1, point[0], point[1]);
				dBox4.draw(ren, tex);

			}
			if (showResOptions == true)
			{
				text.drawMyText(L"800x600", (resOp0[0] + resOp0[2]) / 2, (resOp0[1] + resOp0[3]) / 2, black);
				text.drawMyText(L"1024x768", (resOp1[0] + resOp1[2]) / 2, (resOp1[1] + resOp1[3]) / 2, black);
				text.drawMyText(L"1280x800", (resOp2[0] + resOp2[2]) / 2, (resOp2[1] + resOp2[3]) / 2, black);
				text.drawMyText(L"1280x1024", (resOp3[0] + resOp3[2]) / 2, (resOp3[1] + resOp3[3]) / 2, black);
				text.drawMyText(L"1366x768", (resOp4[0] + resOp4[2]) / 2, (resOp4[1] + resOp4[3]) / 2, black);
				text.drawMyText(L"1440x900", (resOp5[0] + resOp5[2]) / 2, (resOp5[1] + resOp5[3]) / 2, black);
				text.drawMyText(L"1600x900", (resOp6[0] + resOp6[2]) / 2, (resOp6[1] + resOp6[3]) / 2, black);
				text.drawMyText(L"1920x1080", (resOp7[0] + resOp7[2]) / 2, (resOp7[1] + resOp7[3]) / 2, black);
				text.drawMyText(L"3840x2160", (resOp8[0] + resOp8[2]) / 2, (resOp8[1] + resOp8[3]) / 2, black);
			}

			ren.resetFrame();
			cursor.update(ren, playerView.getAim());
			cursor.draw(ren, tex);

			
			if (showResOptions == true)
			{
				if (point[0] < resOp0[0] && point[1] < resOp8[1] && point[0]
					> resOp0[2] && point[1] > resOp0[3] && ((GetKeyState(VK_LBUTTON) & 0x100) != 0))
				{
					showResOptions = false;
				}
			}
			

			ren.endFrame();
		}

		if (gamemode == 2)
		{
			// Light novel portion.
			cursor.update(ren, playerView.getAim());

			if (((GetKeyState(VK_SPACE) & 0x80) != 0) & (delay <= 0)) {
				delay = 25;
				if (showTextBox)
				{
					showTextBox = false;
				}
				else
				{
					showTextBox = true;
				}
			}
			if (delay > 0)
			{
				delay--;
			}
			float* point = playerView.getAim();
		
			ren.beginFrame();
			backdrop.draw(ren, tex);
			pose.update();
			pose.draw(ren, tex);

			if ((showTextBox) & (transistor.changedScene))
			{
				dBox1.update(ren, dialogBox, 14, point[0], point[1]);
				dBox1.draw(ren, tex);
				dBox2.update(ren, optionsBox, 15, point[0], point[1]);
				dBox2.draw(ren, tex);
				dBox3.update(ren, saveBox, 15, point[0], point[1]);
				dBox3.draw(ren, tex);
				dBox4.update(ren, loadBox, 15, point[0], point[1]);
				dBox4.draw(ren, tex);
				dBox5.update(ren, quitToMain, 15, point[0], point[1]);
				dBox5.draw(ren, tex);

			}

			if ((showTextBox) & (transistor.changedScene))
			{
				gamemode = storyReader.execCommand(music, text, miniMan, choice, pose, backdrop, transistor, time);

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
					text.tempMessage(50, 0, 0.9f, L"Saved.", { 211.0f / 255.0f, 145.0f / 255.0f, 1, 1 });
					savefile.Save(storyReader, text, pose, backdrop);
				}

				if (point[0] > loadBox[0] && point[1] > loadBox[1] && point[0] < loadBox[2] && point[1] < loadBox[3] && playerView.getMouse()) {
					text.tempMessage(50, 0, 0.9f, L"Loaded.", { 211.0f / 255.0f, 145.0f / 255.0f, 1, 1 });
					pose.exitAll();
					savefile.Load(storyReader, text, pose, transistor, time);
					storyReader.getNextCommand();
				}
			
				text.drawMyText(L"Save", (saveBox[0] + saveBox[2]) / 2, ((saveBox[1] + saveBox[3]) / 2) , { 0, 0, 0, 1 });
				text.drawMyText(L"Load", (loadBox[0] + loadBox[2]) / 2, ((saveBox[1] + saveBox[3]) / 2) , { 0, 0, 0, 1 });
				text.drawMyText(L"Conf", (optionsBox[0] + optionsBox[2]) / 2, ((optionsBox[1] + optionsBox[3]) / 2) , { 0, 0, 0, 1 });
				text.drawMyText(L"Quit", (quitToMain[0] + quitToMain[2]) / 2, ((saveBox[1] + saveBox[3]) / 2) , { 0, 0, 0, 1 });
				
				text.update();
			}

			ren.resetFrame();
			cursor.update(ren, playerView.getAim());
			cursor.draw(ren, tex);
			transistor.update(backdrop, time);
			transistor.draw(ren, tex);
			ren.endFrame();
			if (gamemode == 3) {
				// Initiate minigame.
				gameLogic.game = miniMan.game;
				gameLogic.init(ren, time);
				playerView.init();
			}
		}

		if (gamemode == 3)
		{
			// Minigame
			playerView.update(screenWidth, screenHeight, xPosRelative, yPosRelative, time, window.getHandle());

			ren.beginFrame();

			gameLogic.update(text, ren, playerView, time, tex, miniMan);
			miniMan.update(time, gameLogic.dataObj, text);
			
			if (miniMan.gameState() == true)
			{
				// Minigame has finished.
				storyReader.minigameReturn(miniMan.setScene());
				gamemode = 2;
			}

			text.update();

			ren.resetFrame();
			cursor.update(ren, playerView.getAim());
			cursor.draw(ren, tex);

			ren.endFrame();

		}
		
		if (gamemode == 4)
		{
			// Light novel choice.
			cursor.update(ren, playerView.getAim());
			float* point = playerView.getAim();

			ren.beginFrame();
			backdrop.draw(ren, tex);

			choice.update(ren, tex, point[0], point[1]);


			if (choice.madeChoice == true) {
				storyReader.minigameReturn(choice.setScene());
				gamemode = 2;
			}
			
			choice.drawText(text);
			text.update();

			ren.resetFrame();
			cursor.update(ren, playerView.getAim());
			cursor.draw(ren, tex);
			transistor.update(backdrop, time);
			transistor.draw(ren, tex);
			ren.endFrame();
		}
	}
	//ReleaseCapture();
	return 0;
}