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

#include "Obstacles.h"

Obstacles::Obstacles(float posx, float posy, int direction) {

	x = posx;
	y = posy;
	speed = ((rand() % 3) + 1) / 10.0f;
	dir = direction;
	OOB = false;
	touch = false;
	colour = rand() % 6;
	newLoc.vertices[0] = { x, y, 0, 0, 0 };
	newLoc.vertices[1] = { x + 0.025f, y, 0, 1, 0 };
	newLoc.vertices[2] = { x, y - 0.05f, 0, 0, 1 };
	newLoc.vertices[3] = { x + 0.025f, y - 0.05f, 0, 1, 1 };
}
Obstacles::Obstacles(const Obstacles &b2) {
	x = b2.x;
	y = b2.y;
	colour = b2.colour;
	speed = b2.speed;
	dir = b2.dir;
	touch = b2.touch;
	OOB = b2.OOB;
}
Obstacles::~Obstacles() {

}
Obstacles::ArrVer Obstacles::update(float* playerPos, float displacement) {
	float pX1 = playerPos[0] - 0.025f;
	float pX2 = playerPos[0] + 0.025f;
	float pY1 = playerPos[1] - 0.05f;
	float pY2 = playerPos[1] + 0.05f;

	// Eventurally, make cars moving up slower if the player is moving faster.
	if (dir == 1) {
		y += dir * speed * displacement;
	}
	else {
		y += dir * speed * displacement;
	}
	
	newLoc.vertices[0] = { x - 0.05f, y - 0.075f, 0, (colour * 0.1667f ), 0 };
	newLoc.vertices[1] = { x + 0.05f, y - 0.075f, 0, ((colour+1) * 0.1667f), 0 };
	newLoc.vertices[2] = { x - 0.05f, y + 0.075f, 0, (colour * 0.1667f), 1 };
	newLoc.vertices[3] = { x + 0.05f, y + 0.075f, 0, ((colour + 1) * 0.1667f), 1 };
	if (dir > 0) {
		newLoc.vertices[0] = { x - 0.05f, y - 0.075f, 0, (colour * 0.1667f), 1 };
		newLoc.vertices[1] = { x + 0.05f, y - 0.075f, 0, ((colour + 1) * 0.1667f), 1 };
		newLoc.vertices[2] = { x - 0.05f, y + 0.075f, 0, (colour * 0.1667f), 0 };
		newLoc.vertices[3] = { x + 0.05f, y + 0.075f, 0, ((colour + 1) * 0.1667f), 0 };
	}
	
	
	/*
	// Rectangle Collision detection
	if ((x < pX2) & (x+0.05f > pX1) & (y - 0.1f > pY2) & (y < pY1 )) {
		touch = true;
	}
	else {
		touch = false;
	}
	*/

	// Circular collision detection.
	float distance = sqrt(((x - playerPos[0])*(x - playerPos[0])) + ((y - playerPos[1])*(y - playerPos[1])));
	if (distance <= 0.1f)
	{
		touch = true;
	}
	else
	{
		touch = false;
	}


	if (y > 1.2f || y < -1.2f) {
		OOB = true;
	}
	return newLoc;
}
bool Obstacles::checkCollision() {
	// Return result of collision check (touch).
	return touch;

}
float* Obstacles::getPos() {

}
