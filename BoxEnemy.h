#pragma once
#include "Renderer.h"
#include <DirectXMath.h>

class BoxEnemy {
public:
	BoxEnemy(float posx, float posy, Renderer& ren);
	~BoxEnemy();
	void update(float* playerPos);
	void draw(Renderer& ren);
	bool checkCollision();
	
private:
	void createMesh(Renderer& ren);
	float x, y, slope;

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* mIB = nullptr;
	
	struct Vertex {
		float x, y, z;
		float r, g, b, a;
	};

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