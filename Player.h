#pragma once
#include "Renderer.h"

class Player {
public:
	Player(Renderer& ren);
	~Player();
	void update(Renderer& renderer, float* pos);
	void draw(Renderer& renderer);


private:

	void createMesh(Renderer& renderer);
	ID3D11Buffer* m_pVertexBuffer = nullptr;

};