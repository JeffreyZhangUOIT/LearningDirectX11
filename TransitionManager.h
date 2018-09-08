#pragma once
#include "Renderer.h"
#include "TextureManager.h"
#include "BackgroundManager.h"
#include "Timer.h"

class Transition
{
public:
	Transition(Renderer& ren);
	~Transition();

	void update(Background& backdrop, Timer& time);
	void draw(Renderer& ren, TextureManager& tex);
	void init(int s, Timer& time);
	bool changedScene;
private:
	float remainingTime;
	float endTime;
	
	int scene;
	Renderer::Vertex foreground[4];
	ID3D11Buffer * mVB;
	ID3D11Buffer * mIB;
};