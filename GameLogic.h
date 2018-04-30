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