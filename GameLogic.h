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
	void enemyConductor(Renderer& ren, float* pos);
	void spawnBoxEnemy(float x, float y, Renderer& ren);
	bool playerDamage();

private:
	void createMeshofBullets(Renderer& ren);
	void drawBullets(Renderer& ren, Bullet::ArrVer vertices);
	void createMeshofBoxEnemies(Renderer& ren);
	void drawBoxEnemies(Renderer& ren, BoxEnemy::ArrVer vertices);

	ID3D11Buffer * m_pBulletVertexBuffer = nullptr;
	ID3D11Buffer* m_pBulletIndexBuffer = nullptr;
	ID3D11Buffer * m_pBoxEnemyVertexBuffer = nullptr;
	ID3D11Buffer* m_pBoxEnemyIndexBuffer = nullptr;
	int delay = 0;
	void killEnemy(BoxEnemy ** box);
	int difficulty = 2;

	std::vector<Bullet> collection;

	Bullet::Vertex m_pVertices[4] = {
		{ 1, 0, 0, 1, 1, 1, 1 },
	{ 0, 1, 0, 1, 1, 1, 1 },
	{ 0, 0, 0, 1, 1, 1, 1 },
	{ 1, 1, 0, 1, 1, 1, 1 }
	};

};