#pragma once
#include <wrl.h>

class GameView {

public:
	GameView(int x);
	~GameView();
	void init();
	float* changeAim(int sw, int sh);
	float* changePos();
	float* getPos();
	float* getAim();

private:
	float* m_pAim = nullptr;
	float* m_pPos = nullptr;
	float x, y, slope;
	POINT cursorPos;
};
