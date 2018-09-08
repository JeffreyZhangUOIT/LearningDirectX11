/*
Copyright 2017, 2017 Jeffrey Zhang

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
#include "Tile.h"

Tile::Tile(float posX, float posY, int gX, int gY, int type)
{
	x = posX;
	y = posY;
	goalX = gX;
	goalY = gY;
	distance = 1.0f;
	switch (type) {
	case -1:
		TCX = 0;
		TCY = 0;
		break;
	case 0:
		TCX = 0.111f;
		TCY = 0;
		break;
	case 1:
		TCX = 0.222f;
		TCY = 0;
		break;
	case 2:
		TCX = 0.333f;
		TCY = 0;
		break;
	case 3:
		TCX = 0.444f;
		TCY = 0;
		break;
	case 4:
		TCX = 0.555f;
		TCY = 0;
		break;
	case 5:
		TCX = 0.666f;
		TCY = 0;
		break;
	case 6:
		TCX = 0.778f;
		TCY = 0;
		break;
	case 7:
		TCX = 0.889f;
		TCY = 0;
		break;
	default:
		TCX = 0.11f;
		TCY = 0;
		break;
	}
	TCX = 0;
	TCY = 0;
}

Tile::Tile(const Tile &bul) {

	x = bul.x;
	y = bul.y;
	goalX = bul.goalX;
	goalY = bul.goalY;
	distance = bul.distance;
	TCX = bul.TCX;
	TCY = bul.TCY;
}

Tile::~Tile()
{

}

Renderer::SquareObj Tile::update(float displacement, int type, bool top)
{
	float locX = -0.7f + ((goalX % 15) * 0.1f);
	float locY = -0.8f + (goalY * 0.1f);

	if ((x - locX) > 0) {
		x -= (1.5f * displacement);
	}
	if ((x - locX) < 0) {
		x += (1.5f * displacement);
	}
	if ((y - locY) > 0) {
		y -= (1.5f * displacement);
	}
	if ((y - locY) < 0) {
		y += (1.5f * displacement);
	}

	distance = sqrt(((x - locX) * (x - locX)) + ((y - locY) * (y - locY)));

	if (distance < 0.05f) {
		still = true;
	}

	else {
		still = false;
	}

	if ((distance < 0.15f) || top) {
		switch (type) {
		case -1:
			TCX = 0;
			TCY = 0;
			break;
		case 0:
			TCX = 0.111f;
			TCY = 0;
			break;
		case 1:
			TCX = 0.222f;
			TCY = 0;
			break;
		case 2:
			TCX = 0.333f;
			TCY = 0;
			break;
		case 3:
			TCX = 0.444f;
			TCY = 0;
			break;
		case 4:
			TCX = 0.555f;
			TCY = 0;
			break;
		case 5:
			TCX = 0.666f;
			TCY = 0;
			break;
		case 6:
			TCX = 0.778f;
			TCY = 0;
			break;
		case 7:
			TCX = 0.889f;
			TCY = 0;
			break;
		default:
			TCX = 0.11f;
			TCY = 0;
			break;
		}
	}

	Renderer::SquareObj newVertices;
	newVertices.vertices[0] = { x - 0.025f, y - 0.025f, 0, TCX, TCY + 1 };
	newVertices.vertices[1] = { x + 0.025f, y - 0.025f, 0, TCX + 0.11f, TCY + 1 };
	newVertices.vertices[2] = { x - 0.025f, y + 0.025f, 0, TCX, TCY };
	newVertices.vertices[3] = { x + 0.025f, y + 0.025f, 0, TCX + 0.11f, TCY };

	return newVertices;
}


