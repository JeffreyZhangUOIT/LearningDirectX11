#include "GameLogic.h"

GameLogic::GameLogic(Renderer& ren) {
	initTDS(ren);
	
}

GameLogic::~GameLogic() {
	delete b1; delete b2; delete b3; delete b4;
	for (int k = 0; k < 100; k++) {
		if (bulletArray[k] != nullptr) {
			delete bulletArray[k];
			bulletArray[k] = nullptr;
		}
	}
	free(bulletArray);
}

void GameLogic::spawnBullet(bool mDown, float* pos, Renderer& ren) 
{
	if (mDown &&(delay <= 0) ){
		for (int k = 0; k < 100; k++) {
			if (bulletArray[k] == nullptr) {
				bulletArray[k] = new Bullet(pos, ren);
				delay = 50;
				break;
			}
		}
	}
}

void GameLogic::destroyBullet(int k)
{
	delete bulletArray[k];
}
void GameLogic::updateTDS(float* pos) {
	if (delay > 0) {
		delay--;
	}
	for (int k = 0; k < 100; k++) {
		bool outOfBounds = false;
		if (bulletArray[k] != nullptr) {
			 outOfBounds = bulletArray[k]->update();
		}
		if (outOfBounds) {
			destroyBullet(k);
		}
	}
	b1->update(pos);
	b2->update(pos);
	b3->update(pos);
	b4->update(pos);
		
}

void GameLogic::drawTDS(Renderer& ren) {
	for (int k = 0; k < 100; k++) {
		if (bulletArray[k] != nullptr) {
			bulletArray[k]->draw(ren);
		}
	}
	b1->draw(ren);
	b2->draw(ren);
	b3->draw(ren);
	b4->draw(ren);
}

void GameLogic::initTDS(Renderer& ren) {
	if (bulletArray) {
		for (int k = 0; k < 100; k++) {
			if (bulletArray[k]) {
				delete bulletArray[k];
			}
		}
		free(bulletArray);
		bulletArray == nullptr;
	}

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
	bulletArray = (Bullet **) malloc(sizeof(Bullet*) * 100);
	for (int k = 0; k < 100; k++) {
		bulletArray[k] = nullptr;
	}
}

bool GameLogic::gameOver() {
	if (b1->checkCollision() || b2->checkCollision() || b3->checkCollision() || b4->checkCollision()) {
		return true;
	}
	return false;
}