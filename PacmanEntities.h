#pragma once
#include "PacmanTiles.h"
#include "PacmanWalker.h"

class PacmanEntites
{
public:
	PacmanEntites(int t, float xpos, float ypos);
	~PacmanEntites();
	Renderer::SquareObj update(PacmanTiles& tiles, Pacman& pacWalker, float displacement);
	float x, y;
	float destX, destY;
	int type, dir;
};