#pragma once
#include "Renderer.h"
#include <DirectXMath.h>

class BoxEnemy {
	
public:
	struct Vertex {
		float x, y, z;
		float r, g, b, a;
	};

	struct ArrVer {
		Vertex vertices[4];
	};

	BoxEnemy(float posx, float posy, Renderer& ren);
	BoxEnemy(const BoxEnemy &b2);
	~BoxEnemy();
	ArrVer update(float* playerPos);
	bool checkCollision();
	int EnemyHp = 2;
	float* getPos();
	bool bulletCollision(float* pos);

private:
	float x, y, slope;

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* mIB = nullptr;

	bool touch;

	Vertex m_pVertices[4] = {
		{ 1, 0, 0, 1, 1, 1, 1 },
		{ 0, 1, 0, 1, 1, 1, 1 },
		{ 0, 0, 0, 1, 1, 1, 1 },
		{ 1, 1, 0, 1, 1, 1, 1 }
	};

	DWORD m_pIndices[6] =
	{
		0, 1, 2,
		1, 2, 3
	};

};