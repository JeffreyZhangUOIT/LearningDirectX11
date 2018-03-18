#include "GameLogic.h"

GameLogic::GameLogic(Renderer& ren) {
	initTDS(ren);
}

GameLogic::~GameLogic() {
	collection.clear();
	boxes.clear();
}

void GameLogic::spawnBullet(bool mDown, float* pos, float* aim,  Renderer& ren)
{
	if (((GetKeyState(VK_LBUTTON) & 0x80) != 0) & delay <= 0)
	{
		collection.emplace_back(pos, ren);
		delay = 3;
		
	}

}

void GameLogic::updateTDS( Renderer& ren, float* pos) {

	enemyConductor(ren, pos);
	if (delay > 0) {
		delay--;
	}

	for (std::vector<Bullet>::iterator bul = collection.begin(); bul != collection.end();)
	{

		bul->update();
		bool outOfBounds = false;
		outOfBounds = bul->OOB;

		if (outOfBounds)
		{
			bul = collection.erase(bul);
		}
		else
		{
			bul++;
		}

	}

	for (std::vector<BoxEnemy>::iterator it = boxes.begin(); it != boxes.end();)
	{
		it->update(pos);
		for (std::vector<Bullet>::iterator bul = collection.begin(); bul != collection.end();)
		{
			float * bPos = bul->getPos();
			
			if (it->bulletCollision(bPos))
			{
				bul = collection.erase(bul);
			}
			
			else
			{
				bul++;
			}
		}

		if (it->EnemyHp <= 0) {
			it = boxes.erase(it);
		}
		else {
			it++;
		}
	}
}


void GameLogic::spawnBoxEnemy(float x, float y, Renderer& ren) {
	boxes.emplace_back(x, y, ren);
	
}

void GameLogic::drawTDS(Renderer& ren) 
{

	for (std::vector<Bullet>::iterator it = collection.begin(); it != collection.end(); ++it)
	{
		it->draw(ren);
	}

	for (std::vector<BoxEnemy>::iterator it = boxes.begin(); it != boxes.end(); ++it)
	{
		it->draw(ren);
	}

}

void GameLogic::initTDS(Renderer& ren) {
	boxes.clear();
	collection.clear();


	srand(static_cast <unsigned> (time(0)));

}

bool GameLogic::playerDamage() {
	
	for (std::vector<BoxEnemy>::iterator it = boxes.begin(); it != boxes.end();)
	{
		if (it->checkCollision()) {
			boxes.erase(it);
			return true;
		}
		else
		{
			it++;
		}
	}
	return false;
}

void GameLogic::enemyConductor(Renderer& ren, float* pos) {
	float x, y;
	x = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.6f))) - 1.3f;
	y = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.6f))) - 1.3f;
	if (boxes.size() < (difficulty * 4)) {
		if (((fabs(x - pos[0]) > 0.3f) || (fabs(x - pos[0]) > 0.3f))) {
			spawnBoxEnemy(x, y, ren);
		}
	}
}

