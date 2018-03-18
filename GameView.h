#pragma once
#include <wrl.h>

class GameView {

public:
	GameView(int x);
	~GameView();
	void init();
	void update(int sw, int sh, int mouseX, int mouseY);
	float* getPos();
	float* getAim();
	void mouseDown(MSG * msg);
	bool getMouse();
	void setPos(float* pos);

private:
	void changeAim(int sw, int sh, int mouseX, int mouseY);
	void changePos();
	bool mDown = false;
	float* m_pAim = nullptr;
	float* m_pPos = nullptr;
	float x, y, slope;
	POINT cursorPos;
};
