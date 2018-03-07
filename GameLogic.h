#pragma once
#include "BoxEnemy.h"
#include "Player.h"
#include "Renderer.h"
#include "Cursor.h"
#include "BoxEnemy.h"
#include "Bullet.h"


class GameLogic {
public:
	GameLogic(Renderer& ren);
	~GameLogic();
	void spawnBullet(bool mDown, float* pos, Renderer& ren);
	void updateTDS(float* pos);
	void initTDS(Renderer& ren);
	void drawTDS(Renderer& ren);
	bool gameOver();

private:
	void destroyBullet(int k);
	int delay = 0;
	Bullet ** bulletArray = nullptr;
	BoxEnemy * b1 = nullptr;
	BoxEnemy * b2 = nullptr;
	BoxEnemy * b3 = nullptr;
	BoxEnemy * b4 = nullptr;

};