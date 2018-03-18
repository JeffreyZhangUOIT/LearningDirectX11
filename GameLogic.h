#pragma once
#include "BoxEnemy.h"
#include "Player.h"
#include "Renderer.h"
#include "Cursor.h"
#include "BoxEnemy.h"
#include "Bullet.h"
#include <vector>
#include <cstdlib>
#include <ctime>

class GameLogic {
public:
	GameLogic(Renderer& ren);
	~GameLogic();

	void updateTDS(Renderer& ren, float* pos);
	void initTDS(Renderer& ren);
	void spawnBullet(bool mDown, float* pos, float* aim, Renderer& ren);
	std::vector<BoxEnemy> boxes;
	void drawTDS(Renderer& ren);
	void enemyConductor(Renderer& ren, float* pos);
	void spawnBoxEnemy(float x, float y, Renderer& ren);
	bool playerDamage();

private:

	int delay = 0;
	void killEnemy(BoxEnemy ** box);
	int difficulty = 2;

	std::vector<Bullet> collection;



};