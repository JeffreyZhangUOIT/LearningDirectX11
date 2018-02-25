#pragma once

#include "Renderer.h"

class Dialog {
public:
	Dialog(Renderer& ren);
	~Dialog();
	void update(Renderer& ren);
	void draw(Renderer& ren);
private:
	void createMesh(Renderer& ren);
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* mIB = nullptr;
	int flag[];
};