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

class Mouse{
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


