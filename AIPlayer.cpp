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

#include "AIPlayer.h"

AIPlayer::AIPlayer(bool isItSmart) {
	random = isItSmart;
	active = false;
}

void AIPlayer::init(bool s, bool on) {
	random = s;
	active = on;
}

void AIPlayer::simMouse(std::vector<BoxEnemy>& boxes, float* pos)
{
	INPUT ip;

	if (!random)
	{
		float x, y;
		float maxDist = 0;
		for (std::vector<BoxEnemy>::iterator it = boxes.begin(); it != boxes.end(); it++)
		{
			float * boxLocation = it->getPos();
			float dist = sqrt(((boxLocation[0] - pos[0]) * (boxLocation[0] - pos[0])) + ((boxLocation[1] - pos[1]) * (boxLocation[1] - pos[1])));
			if (dist > maxDist)
			{
				maxDist = dist;
				x = boxLocation[0]; y = boxLocation[1];
			}
		}
		ip.type = INPUT_MOUSE;
		ip.mi.dx = x;
		ip.mi.dy = y;
		ip.mi.mouseData = 0;
		ip.mi.dwFlags = MOUSEEVENTF_ABSOLUTE;
		ip.mi.time = 0;
		ip.mi.dwExtraInfo = 0;
		SendInput(1, &ip, sizeof(INPUT));
	}
	else
	{
		ip.type = INPUT_MOUSE;
		ip.mi.dx = rand();
		ip.mi.dy = rand();
		ip.mi.mouseData = 0;
		ip.mi.dwFlags = MOUSEEVENTF_ABSOLUTE;
		ip.mi.time = 0;
		ip.mi.dwExtraInfo = 0;
		SendInput(1, &ip, sizeof(INPUT));
	}




}

float* AIPlayer::simKeyboard(std::vector<BoxEnemy>& boxes, float* pos)
{
	INPUT ip;
	float* position = (float*)malloc(sizeof(float) * 4);
	position[0] = 0; position[1] = 0; position[2] = 0; position[3] = 0;
	numEnemies = 0;
	if (!random)
	{
		float x = 0, y = 0;
		
		for (std::vector<BoxEnemy>::iterator it = boxes.begin(); it != boxes.end(); it++)
		{
			numEnemies++;
			float * boxLocation = it->getPos();
			if ((x - boxLocation[0]) != 0)
			{
				x = x + (1 / (pos[0] - boxLocation[0]));
			}
			if ((y - boxLocation[1]) != 0)
			{
				y = y + (1 / (pos[1] - boxLocation[1]));
			}
		}

		if (x < 0) {
			position[0] = 1;
		}

		if (x > 0) {
			position[1] = 1;
		}

		if (y < 0) {
			position[2] = 1;
		}

		if (y > 0) {
			position[3] = 1;
		}
		//return position;
	}

	else
	{

		for (std::vector<BoxEnemy>::iterator it = boxes.begin(); it != boxes.end(); it++)
		{
			numEnemies++;
		}
		position[0] = 1;
		//return position;
	}
	return position;
}