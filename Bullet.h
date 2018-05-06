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

class Bullet {
public:
	Bullet(float* pos, Renderer& ren, Timer& time);  // Constructor
	Bullet(const Bullet &bul); // Copy constructor
	~Bullet();

	struct Vertex {
		float x, y, z;
		float r, g, b, a;
	};

	struct ArrVer {
		struct Vertex vertices[4];
	};

	ArrVer update(float displacement);
	void draw(Renderer& ren);
	bool OOB = false;
	float* getPos();

private:
	void createMesh(Renderer& ren);
	float x, y, dir;
	float * point = nullptr;


};