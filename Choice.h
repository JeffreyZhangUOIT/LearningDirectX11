#pragma once
#include <string.h>
#include <stdlib.h>
#include "Renderer.h"
#include "TextureManager.h"
#include "TextHandler.h"
#include "GameView.h"

class Choice {
public:
	Choice(Renderer& ren);
	~Choice();
	void init(char param[1000]);
	void init2(Renderer& ren);
	void draw(Renderer& ren, TextureManager& tex);
	void drawText(TextHandler& text);
	void update(Renderer& ren, TextureManager& tex, float x, float y);
	char * setScene();
	bool madeChoice;

private:
	Renderer::Vertex box1[4] = 
	{
		{ -0.90f, 0.90f, 0, 0, 0 },
		{ 0.90f, 0.90f, 0, 1, 0 },
		{ -0.90f, -0.50f, 0, 0, 1 },
		{ 0.90f, -0.50f, 0, 1, 1 }
	};
	Renderer::Vertex box2[4] =
	{
		{ -0.90f, 0.90f, 0, 0, 0 },
		{ 0.90f, 0.90f, 0, 1, 0 },
		{ -0.90f, -0.50f, 0, 0, 1 },
		{ 0.90f, -0.50f, 0, 1, 1 }
	};
	Renderer::Vertex box3[4] =
	{
		{ -0.90f, 0.90f, 0, 0, 0 },
		{ 0.90f, 0.90f, 0, 1, 0 },
		{ -0.90f, -0.50f, 0, 0, 1 },
		{ 0.90f, -0.50f, 0, 1, 1 }
	};
	Renderer::Vertex box4[4] =
	{
		{ -0.90f, 0.90f, 0, 0, 0 },
		{ 0.90f, 0.90f, 0, 1, 0 },
		{ -0.90f, -0.50f, 0, 0, 1 },
		{ 0.90f, -0.50f, 0, 1, 1 }
	};

	ID3D11Buffer * m_pVertexBuffer = nullptr;
	ID3D11Buffer * mIB = nullptr;
	int selection;
	int numOfChoices;
	float point[2];
	char choiceBuffer[1000];
	char choice1[256];
	char choice2[256];
	char choice3[256];
	char choice4[256];
	char sceneBuffer[500];
	int size;
	wchar_t * buffer;
	bool uninitDevice;
};