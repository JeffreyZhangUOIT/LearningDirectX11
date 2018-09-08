#pragma once
#include "Renderer.h"
#include "TextureManager.h"
class PoseArtManager {
public:
	PoseArtManager(Renderer& ren);
	~PoseArtManager();
	void update();
	void draw(Renderer& ren, TextureManager& tex);
	void enter(int person); // Actually performs a swap with oldest slot.
	void exit(int person);
	void exitAll();
	int q1, q2;

private:
	void createMesh(Renderer& ren);
	float x1, x2, y1, y2;
	float g1x, g2x, g1y, g2y;
	bool enterLeft, enterRight, leftOldest;
	
	float t1X, t2X, t1Y, t2Y;
	int p1t, p2t;

	Renderer::Vertex box1[4] =
	{
		{ 1, 0, 0, 0, 0 },
		{ 0, 1, 0, 0, 1 },
		{ 0, 0, 0, 1, 0 },
		{ 1, 1, 0, 1, 1 }
	};

	Renderer::Vertex box2[4] =
	{
		{ 1, 0, 0, 0, 0 },
		{ 0, 1, 0, 0, 1 },
		{ 0, 0, 0, 1, 0 },
		{ 1, 1, 0, 1, 1 }
	};
	ID3D11Buffer * mVB;
	ID3D11Buffer * mVB2;
	ID3D11Buffer * mIB;

};