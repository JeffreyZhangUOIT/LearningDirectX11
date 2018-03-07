#pragma once

#include "Renderer.h"

class Bullet {
public:
	Bullet(float* pos, Renderer& ren);
	~Bullet();
	bool update();
	void draw(Renderer& ren);

private:
	struct Vertex {
		float x, y, z;
		float r, g, b, a;
	};

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* mIB = nullptr;

	void createMesh(Renderer& ren);
	float x, y, dir;

	Vertex m_pVertices[4] = {
		{ 0, 0, 0, 1, 1, 1, 1 },
		{ 0, 1, 0, 1, 1, 1, 1 },
		{ 1, 0, 0, 1, 1, 1, 1 },
		{ 1, 1, 0, 1, 1, 1, 1 }
	};

	DWORD m_pIndices[6] =
	{
		2, 1, 0,
		1, 2, 3
	};



};