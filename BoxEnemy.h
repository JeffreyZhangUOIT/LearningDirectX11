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
along with ProjectFiasco.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once
#include "Renderer.h"
#include "Timer.h"
#include <DirectXMath.h>

class BoxEnemy {
	
public:
	struct ArrVer {
		Renderer::Vertex vertices[8];
	};
	BoxEnemy(float posx, float posy);
	BoxEnemy(const BoxEnemy &b2);
	~BoxEnemy();
	ArrVer update(float* playerPos, float time);
	bool checkCollision();
	int EnemyHp = 5;
	float* getPos();
	float point[2];
	bool bulletCollision(float* pos);

private:
	float x, y, slope;
	bool touch;

};