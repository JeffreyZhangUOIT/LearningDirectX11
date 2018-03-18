#pragma once
#include "Renderer.h"

class Player {
public:
	Player(Renderer& ren);
	~Player();
	void update(Renderer& renderer, float* pos, bool dmg);
	void draw(Renderer& renderer);
	void init();
	bool alive;
	int playerHp = 3;

private:
	
	int immunity = 0;
	
	void createMesh(Renderer& renderer);
	ID3D11Buffer* m_pVertexBuffer = nullptr;

};