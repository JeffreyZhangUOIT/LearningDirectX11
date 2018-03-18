#pragma once
#include <vector>
#include "BoxEnemy.h"

class AIPlayer {
public:
	AIPlayer(bool isItSmart);
	~AIPlayer() {}
	void init(bool isItSmart, bool isItOn);
	//void update(std::vector<BoxEnemy>& boxes, float* pos);
	void simMouse(std::vector<BoxEnemy>& boxes, float* pos);
	float* simKeyboard( std::vector<BoxEnemy>& boxes, float* pos);
	bool active = false;
	int numEnemies = 0;
private:

	bool random;
	
};