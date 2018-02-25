#pragma once
#include "Renderer.h"
#include <DirectXMath.h>

class BoxEnemy {
public:
	BoxEnemy(float posx, float posy, Renderer& ren);
	~BoxEnemy();
	void update(Renderer& ren, float* playerPos);
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


};