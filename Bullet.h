#pragma once

#include "Renderer.h"
#include <DirectXMath.h>

class Bullet {
public:
	Bullet(float* pos, Renderer& ren);  // Constructor
	Bullet(const Bullet &bul); // Copy constructor
	~Bullet();

	struct Vertex {
		float x, y, z;
		float r, g, b, a;
	};

	struct ArrVer {
		struct Vertex vertices[4];
	};

	ArrVer update();
	void draw(Renderer& ren);
	bool OOB = false;
	float* getPos();

private:
	void createMesh(Renderer& ren);
	float x, y, dir;
	float * point = nullptr;
	
};