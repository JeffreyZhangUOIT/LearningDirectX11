#pragma once

#include "Renderer.h"
#include "TextureManager.h"
#include "PacmanTiles.h"
#include <cmath>
class Pacman
{
public:
	Pacman();
	~Pacman();
	void update(PacmanTiles& pacTiles, float displacement);
	void draw(Renderer& ren, TextureManager& tex);
	void modSpeed(bool fast);
	void createMesh(Renderer& ren);
	void init();
	float x, y;

private:
	ID3D11Buffer * mVB;
	ID3D11Buffer * mIB;
	int direction;
	float destX, destY;
	float speedModifer;
	float rangeModifer;
	float speedTicker;
	float rangeTicker;
};