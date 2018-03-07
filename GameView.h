#pragma once
#include <wrl.h>

class GameView {

public:
	GameView(int x);
	~GameView();
	void init();
	void update(int sw, int sh);
	float* changeAim(int sw, int sh);
	float* changePos();
	float* getPos();
	float* getAim();
	void mouseDown(MSG msg);
	
	bool getMouse();

private:
	bool mDown = false;
	float* m_pAim = nullptr;
	float* m_pPos = nullptr;
	float x, y, slope;
	POINT cursorPos;
};
