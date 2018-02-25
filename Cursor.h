#pragma once
#pragma once
#include "Renderer.h"
#include "GameObject.h"

class Mouse : public GameObject {
public:
	Mouse(Renderer& ren);
	~Mouse();
	void update(Renderer& ren, float* aim);
	void draw(Renderer& ren);
	void createMesh(Renderer& ren);
	float x, y;

private:
	POINT cursorPos;
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* mIB = nullptr;
};


