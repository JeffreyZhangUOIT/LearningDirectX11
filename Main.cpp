#include <windows.h>
#include "Renderer.h"
#include "Player.h"
#include "Cursor.h"
#include "GameObject.h"
#include "GameView.h"
#include "Timer.h"
#include "Dialog.h"
#include "GameLogic.h"
#include "AIPlayer.h"
#include "Bullet.h"
#include <tchar.h>
#include <string>
#include <iostream>
#include <fstream>

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
	Dialog dBox1(ren), dBox2(ren), dBox3(ren), dBox4(ren);
	HDC hdc = GetDC(window.getHandle());
	GameLogic gameLogic(ren);
	MSG msg = { 0 };
	int itr = 0;
	FILE* fp;
	fopen_s(&fp, "botOutput.txt", "a");
	AIPlayer bot(false); 
	Timer time;
	//Bullet bill(playerView.getPos(), ren);
	time.resetTime();
	
	//Text myText(ren);
	int gamemode = 1;
	//SetCapture(window.getHandle());
	int xPosRelative = 0, yPosRelative = 0;
	ShowCursor(false);

	float* startGameBox = (float*) malloc(sizeof(float) * 4);
	startGameBox[0] = -0.8f; startGameBox[1] = -0.7f; startGameBox[2] = -0.5f; startGameBox[3] = -0.5f;
	int* textBox1 = convertFloatToLogicalUnits(startGameBox, screenWidth, screenHeight);

	float* badAIBox = (float*)malloc(sizeof(float) * 4);
	badAIBox[0] = -0.4f; badAIBox[1] = -0.7f; badAIBox[2] = -0.1f; badAIBox[3] = -0.5f;
	int* textBox2 = convertFloatToLogicalUnits(badAIBox, screenWidth, screenHeight);

	float* goodAIBox = (float*)malloc(sizeof(float) * 4);
	goodAIBox[0] = 0; goodAIBox[1] = -0.7f; goodAIBox[2] = 0.3f; goodAIBox[3] = -0.5f;
	int* textBox3 = convertFloatToLogicalUnits(goodAIBox, screenWidth, screenHeight);

	float* quitBox = (float*)malloc(sizeof(float) * 4);
	quitBox[0] = 0.4f; quitBox[1] = -0.7f; quitBox[2] = 0.7f; quitBox[3] = -0.5f;
	int* textBox4 = convertFloatToLogicalUnits(quitBox, screenWidth, screenHeight);

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

			else if (msg.message == WM_QUIT)
			{
				break;
			}

			if (!fHandled)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		// ... Main loop ...
		if (gamemode == 0) 
		{
			ren.beginFrame();
			gameLogic.spawnBullet(playerView.getMouse(), playerView.getPos(), playerView.getAim(), ren);
			gameLogic.updateTDS(ren, playerView.getPos());

			cursor.update(ren, playerView.getAim());
			cursor.draw(ren);

			player.update(ren, playerView.getPos(), gameLogic.playerDamage());
			
			if (bot.active) {
				playerView.setPos(bot.simKeyboard(gameLogic.boxes, playerView.getPos()));
				std::string str("Number of Enemies: ");
				str += std::to_string(bot.numEnemies);
				str += " Elapsed Time: ";
				str += std::to_string(deltaT);
				str += " Player Hp: ";
				str += std::to_string(player.playerHp);
				str += " Iteration: ";
				str += std::to_string(itr);
				TextOut(hdc,  0, 0, str.c_str(), str.length());
			}
			if (!player.alive && bot.active) {
				gamemode = 2;
			}
			else if (!player.alive && !bot.active) {
				gamemode = 1;
			}
			if (itr > 4) {
				gamemode = 3;
			}
			
			player.draw(ren);

			ren.endFrame();
		}


		if (gamemode == 1) 
		{
			
			bot.init(false, false);
			// Update
			cursor.update(ren, playerView.getAim());
			float* point = playerView.getAim();
			TextOut(hdc, screenWidth/2 , screenHeight /2 , "Just Survive.", 12);
			TextOut(hdc, textBox1[0] + 100, textBox1[1] - 25, "Game Start", 10);
			TextOut(hdc, textBox2[0] + 50, textBox3[1] - 25, "BadAIAutoTest", 13);
			TextOut(hdc, textBox3[0] + 50, textBox2[1] - 25, "GoodAIAutoTest", 14);
			TextOut(hdc, textBox4[0] + 100, textBox4[1] - 25, "Quit", 4);
			if (point[0] > startGameBox[0] && point[1] > startGameBox[1] && point[0] < startGameBox[2] && point[1] < startGameBox[3] && playerView.getMouse()) { // check if cursor is in the damn box. + click.
				gameLogic.initTDS(ren);
				bot.init(false, false);
				playerView.init();
				player.init();
				gamemode = 0;
			}

			if (point[0] > badAIBox[0] && point[1] > badAIBox[1] && point[0] < badAIBox[2] && point[1] < badAIBox[3] && playerView.getMouse()) { // check if cursor is in the damn box. + click.
				gameLogic.initTDS(ren);
				bot.init(true, true);
				playerView.init();
				player.init();
				gamemode = 0;
			}

			if (point[0] > goodAIBox[0] && point[1] > goodAIBox[1] && point[0] < goodAIBox[2] && point[1] < goodAIBox[3] && playerView.getMouse()) { // check if cursor is in the damn box. + click.
				gameLogic.initTDS(ren);
				bot.init(false, true);
				playerView.init();
				player.init();
				gamemode = 0;
			}

			if (point[0] > quitBox[0] && point[1] > quitBox[1] && point[0] < quitBox[2] && point[1] < quitBox[3] && playerView.getMouse()) { // check if cursor is in the damn box. + click.
				break;
			}

			// Draw
			ren.beginFrame();

			dBox1.update(ren, startGameBox);
			dBox1.draw(ren);
			dBox2.update(ren, badAIBox);
			dBox2.draw(ren);
			dBox3.update(ren, goodAIBox);
			dBox3.draw(ren);
			dBox4.update(ren, quitBox);
			dBox4.draw(ren);
			
			cursor.draw(ren);
			ren.endFrame();
			
			// The text doesn't work yet, need to get them to render to the same surface.
			//const wchar_t* string = L"Some Testing Text";
			//myText.DrawMyText(50.f, 400.f, 750.f, 550.f, string, 0, ren);

		}	
		if (gamemode == 2) 
		{

			TextOut(hdc, screenWidth / 2, screenHeight / 2, "The bot died. Stats Logged. Restarting.", 39);
			int numE = bot.numEnemies; unsigned long et = deltaT; int ph = player.playerHp;
			std::string str("Number of Enemies: ");
			str += std::to_string(numE);
			str += " Elapsed Time: ";
			str += std::to_string(et);
			str += " Player Hp: ";
			str += std::to_string(ph);
			str += " Iteration: ";
			str += std::to_string(itr);
			fprintf(fp, "%s\n", str.c_str());
			gameLogic.initTDS(ren);
			playerView.init();
			player.init();
			itr++;
			gamemode = 0;
		}

		if (gamemode == 3)
		{
			fclose(fp);
			break;

		}
		
	}
	//ReleaseCapture();
	return 0;
}


