#pragma once

#include "Renderer.h"
#include "TextureManager.h"
#include <string>
#include <stdio.h> 

class PacmanTiles
{
public:
	PacmanTiles();
	~PacmanTiles();
	void initBG1(Renderer& ren);
	bool checkSquareArea(float x, float y);
	bool checkTile(float x, float y);
	void drawBoard(Renderer& ren, TextureManager& tex); // Debug tool.
private:
	int board[930];
	FILE * boardInfo;
	ID3D11Buffer * mIB;
	ID3D11Buffer * mVB;
};