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
#include "GameView.h"
#include <math.h>

GameView::GameView(int ax){
	x = ax;
	init();
	m_pAim[0] = 0;
	m_pAim[1] = 0;

}

GameView::~GameView() {
	delete m_pAim;
	delete m_pPos;
}

void GameView::update(int sw, int sh, int mouseX, int mouseY) {
	mDown = false;
	changeAim(sw, sh, mouseX, mouseY);
	changePos();
}

void GameView::setPos(float* pos) {
	slope = atan2((m_pAim[0] - x), (m_pAim[1] - y));

	if (pos[0] == 1) {
		y += 0.01f;
	}
	if (pos[1] == 1) {
		y -= 0.01f;
	}
	if (pos[3] == 1) {
		x += 0.01f;
	}
	if (pos[2] == 1) {
		x -= 0.01f;
	}

	if (x < -1.15f) {
		x = 1.1f;
	}
	if (x > 1.15f) {
		x = -1.1f;
	}
	if (y > 1.15f) {
		y = -1.1f;
	}
	if (y < -1.15f) {
		y = 1.1f;
	}
	m_pPos[0] = x;
	m_pPos[1] = y;
	m_pPos[2] = slope;

}

void GameView::changeAim(int sw, int sh, int mouseX, int mouseY) {
	GetCursorPos(&cursorPos);
	float mx = ((2.0f * (float)cursorPos.x) / (float)sw) - 1.0f;
	float my = (((2.0f * (float)cursorPos.y) / (float)sh) - 1.0f) * -1.0f;

	m_pAim[0] = (mx);
	m_pAim[1] = (my);

	if (m_pAim[0] < -1) {
		m_pAim[0] = -1;
	}
	if (m_pAim[0] > 1) {
		m_pAim[0] = 1;
	}
	if (m_pAim[1] < -1) {
		m_pAim[1] = -1;
	}
	if (m_pAim[1] > 1) {
		m_pAim[1] = 1;
	}
	

}

void GameView::changePos() {
	slope = atan2((m_pAim[0] - x), (m_pAim[1] - y));

	if (GetAsyncKeyState(0x57)) {
		y += 0.01f;
	}
	if (GetAsyncKeyState(0x53)) {
		y -= 0.01f;
	}
	if (GetAsyncKeyState(0x44)) {
		x += 0.01f;
	}
	if (GetAsyncKeyState(0x41)) {
		x -= 0.01f;
	}

	if (x < -1.15f) {
		x = 1.1f;
	}
	if (x > 1.15f) {
		x = -1.1f;
	}
	if (y > 1.15f) {
		y = -1.1f;
	}
	if (y < -1.15f) {
		y = 1.1f;
	}
	m_pPos[0] = x;
	m_pPos[1] = y;
	m_pPos[2] = slope;

}
float* GameView::getPos() {
	return m_pPos;

}

float* GameView::getAim() {
	return m_pAim;

}
void GameView::mouseDown(MSG *msg) {
	if (msg->message == WM_LBUTTONDOWN) {
		mDown = true;
	}
	else {
		mDown = false;
	}
}
bool GameView::getMouse() {
	return mDown;
}
void GameView::init() {
	if (m_pAim != nullptr) {
		delete m_pAim;
	}
	if (m_pPos != nullptr) {
		delete m_pPos;
	}
	m_pAim = new float[2];
	m_pPos = new float[3];
	m_pAim[0] = 0;
	m_pAim[1] = 0;
	m_pPos[0] = 0;
	m_pPos[1] = 0;
	m_pPos[2] = 0;
	x = 0;
	y = 0;
	slope = 0;
}