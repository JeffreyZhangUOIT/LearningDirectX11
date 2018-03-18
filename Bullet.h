#pragma once

#include "Renderer.h"
#include <DirectXMath.h>

class Bullet {
public:
	Bullet(float* pos, Renderer& ren);  // Constructor
	Bullet(const Bullet &bul); // Copy constructor
	~Bullet();
	void update();
	void draw(Renderer& ren);
	bool OOB = false;
	float* getPos();

private:
	void createMesh(Renderer& ren);
	float x, y, dir;
	float * point = nullptr;

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* mIB = nullptr;

	struct Vertex {
		float x, y, z;
		float r, g, b, a;
	};

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