#pragma once

#include "Renderer.h"
#include "TextureManager.h"

class Background
{
public:
	Background(Renderer& ren);
	~Background();
	void setBackground(int bg);
	void draw(Renderer& ren, TextureManager& tex);
	int currentBackground = 6;
private:
	
	Renderer::Vertex background[4];
	ID3D11Buffer * mVB;
	ID3D11Buffer * mIB;
};