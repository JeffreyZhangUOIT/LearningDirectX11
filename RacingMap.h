#pragma once

#include "Renderer.h"
#include "TextureManager.h"
#include "Timer.h"

class RacingMap {
public:
	RacingMap();
	~RacingMap();
	void draw(Renderer& ren, TextureManager& tex);
	void createMesh(Renderer& ren);
	void update(float displacement);
private:
	ID3D11Buffer * m_pVertexBuffer = nullptr;
	ID3D11Buffer* mIB = nullptr;
	Renderer::Vertex map1[4] = {
		{ -1, 1, .0f, 0, 0 },
		{ 1, 1, .0f, 1, 0 },
		{ -1, -1, .0f, 0, 1 },
		{ 1, -1, .0f, 1, 1 }
	};

	Renderer::Vertex map2[4] = {
		{ -1, 1, .0f, 0, 0 },
		{ 1, 1, .0f, 1, 0 },
		{ -1, -1, .0f, 0, 1 },
		{ 1, -1, .0f, 1, 1 }
	};

	float y1, y2;
};