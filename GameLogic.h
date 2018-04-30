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
#include "BoxEnemy.h"
#include "Player.h"
#include "Renderer.h"
#include "Cursor.h"
#include "BoxEnemy.h"
#include "Bullet.h"
#include "TextHandler.h"
#include <vector>
#include <cstdlib>
#include <ctime>

class GameLogic {
	
public:

	GameLogic(Renderer& ren);
	~GameLogic();

	void updateTDS(TextHandler& text, Renderer& ren, float* pos);
	void initTDS(Renderer& ren);
	void spawnBullet(bool mDown, float* pos, float* aim, Renderer& ren);
	std::vector<BoxEnemy> boxes;
	void enemyConductor(Renderer& ren, float* pos);
	void spawnBoxEnemy(float x, float y, Renderer& ren);
	bool playerDamage();

private:
	void createMeshofBullets(Renderer& ren);
	void drawBullets(Renderer& ren, Bullet::ArrVer vertices);
	void createMeshofBoxEnemies(Renderer& ren);
	void drawBoxEnemies(Renderer& ren, BoxEnemy::ArrVer vertices);
	void killEnemy(BoxEnemy ** box);

	ID3D11Buffer * m_pBulletVertexBuffer = nullptr;
	ID3D11Buffer* m_pBulletIndexBuffer = nullptr;
	ID3D11Buffer * m_pBoxEnemyVertexBuffer = nullptr;
	ID3D11Buffer* m_pBoxEnemyIndexBuffer = nullptr;

	int difficulty = 2;
	int delay = 0;
	int spawnDelay = 0;
	int timeScaledDifficulty = 15 - difficulty;
	int deceasedEnemies = 0;


	std::vector<Bullet> collection;

	Bullet::Vertex m_pVertices[4] = {
		{ 1, 0, 0, 1, 1, 1, 1 },
	{ 0, 1, 0, 1, 1, 1, 1 },
	{ 0, 0, 0, 1, 1, 1, 1 },
	{ 1, 1, 0, 1, 1, 1, 1 }
	};

};