#pragma once
#include "BoxEnemy.h"
#include "Player.h"
#include "Renderer.h"
#include "Cursor.h"
#include "BoxEnemy.h"


class GameLogic {
public:
	GameLogic(Renderer& ren);
	~GameLogic();

	void updateTDS(Renderer& ren, float* pos);
	void initTDS(Renderer& ren);
	void drawTDS(Renderer& ren);
	bool gameOver();

private:
	BoxEnemy * b1 = nullptr;
	BoxEnemy * b2 = nullptr;
	BoxEnemy * b3 = nullptr;
	BoxEnemy * b4 = nullptr;

};