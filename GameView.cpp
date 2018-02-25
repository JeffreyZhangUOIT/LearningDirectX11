#include "GameView.h"
#include <math.h>

GameView::GameView(int ax){
	x = ax;
	init();
}

GameView::~GameView() {
	delete m_pAim;
	delete m_pPos;
}

float* GameView::changeAim(int sw, int sh) {
	GetCursorPos(&cursorPos);
	float mx = ((2.0f * (float)cursorPos.x) / (float)sw) - 1.0f;
	float my = (((2.0f * (float)cursorPos.y) / (float)sh) - 1.0f) * -1.0f;
	m_pAim[0] = mx;
	m_pAim[1] = my;
	return m_pAim;

}

float* GameView::changePos() {
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
	return m_pPos;

}
float* GameView::getPos() {
	return m_pPos;

}

float* GameView::getAim() {
	return m_pAim;

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