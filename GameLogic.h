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
#include "BoxEnemy.h"
#include "GameView.h"
#include "Bullet.h"
#include "Timer.h"
#include "Obstacles.h"
#include "TextHandler.h"
#include "TextureManager.h"
#include "MinigameMetadata.h"
#include "Match3Board.h"
#include "Tile.h"
#include "PacmanWalker.h"
#include "PacmanTiles.h"
#include "PacmanEntities.h"
#include "TextGrabber.h"
#include "RacingMap.h"
#include <vector>
#include <cmath>

class GameLogic {
	
public:

	GameLogic(Renderer& ren);
	~GameLogic();

	void update(TextHandler& text, Renderer& ren, GameView& pv, Timer& time, TextureManager& tex, MinigameManager& miniMan);
	void init(Renderer& ren, Timer& time);
	//void spawnBullet(bool mDown, float* pos, float* aim, Renderer& ren, Timer& time);
	void enemyConductor(Renderer& ren, float* pos, Timer& time);
	bool playerDamage();
	MinigameMetadata dataObj;
	int game;
	std::vector<BoxEnemy> boxes;

private:
	//void createMeshofCars(Renderer& ren);
	void drawCars(Renderer& renderer, Obstacles::ArrVer vertices);
	void createMeshofBullets(Renderer& ren);
	void drawBullets(Renderer& ren, Renderer::SquareObj vertices);
	void createMeshofBoxEnemies(Renderer& ren);
	void drawBoxEnemies(TextureManager& tex, Renderer& ren, BoxEnemy::ArrVer vertices);
	void createMeshofPacEntites(Renderer& ren);
	void drawPacEntities(Renderer& ren, Renderer::SquareObj vertices);
	void killEnemy(BoxEnemy ** box);
	void createMeshForProgress(Renderer& renderer);
	void drawProgressBar(TextureManager& tex, Renderer& renderer, float progress);

	ID3D11Buffer * m_pBulletVertexBuffer = nullptr;
	ID3D11Buffer* m_pBulletIndexBuffer = nullptr;
	ID3D11Buffer * m_pBoxEnemyVertexBuffer = nullptr;
	ID3D11Buffer* m_pBoxEnemyIndexBuffer = nullptr;
	ID3D11Buffer * m_pPacEntitiesVertexBuffer = nullptr;
	ID3D11Buffer* m_pPacEntitiesIndexBuffer = nullptr;

	RacingMap raceMap;
	Player player;
	M3Board board;
	PacmanTiles pacTiles;
	Pacman pacWalker;

	float progress;
	float acceleration;
	float prevFT;
	float currentFT;
	float deltaFT;
	float delay = 0;
	float scoreBuffer;

	// Top-Down Shooter Specific var
	float timeSinceLastSpawn = 0;

	std::vector<Bullet> collection;
	std::vector<Obstacles> automobiles;
	std::vector<PacmanEntites> pacEntities;

	Renderer::Vertex m_pVertices[4] = 
	{
		{ 1, 0, 0, 0, 0 },
		{ 0, 1, 0, 0, 1 },
		{ 0, 0, 0, 1, 0 },
		{ 1, 1, 0, 1, 1 }
	};

};