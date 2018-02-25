#include "GameLogic.h"

GameLogic::GameLogic(Renderer& ren) {
	initTDS(ren);
	
}

GameLogic::~GameLogic() {
	delete b1; delete b2; delete b3; delete b4;
}

void GameLogic::updateTDS( Renderer& ren, float* pos) {
	b1->update(ren, pos);
	b2->update(ren, pos);
	b3->update(ren, pos);
	b4->update(ren, pos);
		
}

void GameLogic::drawTDS(Renderer& ren) {
	b1->draw(ren);
	b2->draw(ren);
	b3->draw(ren);
	b4->draw(ren);
}

void GameLogic::initTDS(Renderer& ren) {
	if (b1 != nullptr) {
		delete b1;
	}
	if (b2 != nullptr) {
		delete b2;
	}
	if (b3 != nullptr) {
		delete b3;
	}
	if (b4 != nullptr) {
		delete b4;
	}
	b1 = new BoxEnemy(-1, -1, ren);
	b2 = new BoxEnemy(1, -1, ren);
	b3 = new BoxEnemy(-1, 1, ren);
	b4 = new BoxEnemy(1, 1, ren);
}

bool GameLogic::gameOver() {
	if (b1->checkCollision() || b2->checkCollision() || b3->checkCollision() || b4->checkCollision()) {
		return true;
	}
	return false;
}