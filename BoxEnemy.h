/*
Copyright 2017, 2018 Jeffrey Zhang

This file is part of ProjectFiasco.

ProjectFiasco is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ProjectFiasco is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once
#include "Renderer.h"
#include <DirectXMath.h>

class BoxEnemy {
	
public:
	struct Vertex {
		float x, y, z;
		float r, g, b, a;
	};

	struct ArrVer {
		Vertex vertices[8];
	};

	BoxEnemy(float posx, float posy, Renderer& ren);
	BoxEnemy(const BoxEnemy &b2);
	~BoxEnemy();
	ArrVer update(float* playerPos);
	bool checkCollision();
	int EnemyHp = 5;
	float* getPos();
	bool bulletCollision(float* pos);

private:
	float x, y, slope;

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* mIB = nullptr;

	bool touch;

	Vertex m_pVertices[4] = {
		{ 1, 0, 0, 1, 1, 1, 1 },
		{ 0, 1, 0, 1, 1, 1, 1 },
		{ 0, 0, 0, 1, 1, 1, 1 },
		{ 1, 1, 0, 1, 1, 1, 1 }
	};

	DWORD m_pIndices[4] =
	{
		0, 1, 2,
		3
	};

};