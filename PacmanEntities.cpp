#include "PacmanEntities.h"

PacmanEntites::PacmanEntites(int t, float xpos, float ypos) 
{
	x = xpos;
	y = ypos;
	destX = x; 
	destY = y;
	type = t;
}

PacmanEntites::~PacmanEntites()
{

}

Renderer::SquareObj PacmanEntites::update(PacmanTiles& tiles, Pacman& pacWalker, float displacement)
{

	// Center the MC, update power up durations, and move the player towards the their destination.

	if (tiles.checkSquareArea(((std::floor(destX / 0.0625f)) *0.0625f) + 0.06f, destY) == true)
	{
		destX = ((std::floor(destX / 0.0625f)) *0.0625f) + 0.06f;
	}

	if (x < destX) {
		x += 0.4f * displacement ;
	}

	if (x > destX) {
		x -= 0.4f * displacement ;
	}

	if (y < destY) {
		y += 0.65f * displacement ;
	}

	if (y > destY) {
		y -= 0.65f * displacement ;
	}

	// 0 = speed up / 1 = speed down / 2 = student
	if (type == 2)
	{
		float distance = sqrt(((x - pacWalker.x) * (x - pacWalker.x)) + (y - pacWalker.y)*(y - pacWalker.y));
		if (distance <= 0.5f)
		{
			float d1 = sqrt((((x + 0.0625f) - pacWalker.x) * (x + 0.0625f - pacWalker.x)) + (y - pacWalker.y)*(y - pacWalker.y));
			float d2 = sqrt(((x - 0.0625f - pacWalker.x) * (x - 0.0625f - pacWalker.x)) + (y - pacWalker.y)*(y - pacWalker.y));
			float d3 = sqrt(((x - pacWalker.x) * (x - pacWalker.x)) + (y + 0.0689655f - pacWalker.y)*(y + 0.0689655f - pacWalker.y));
			float d4 = sqrt(((x - pacWalker.x) * (x - pacWalker.x)) + (y - 0.0689655f - pacWalker.y)*(y - 0.0689655f - pacWalker.y));

			if (tiles.checkSquareArea(x + 0.0625f, y) == false)
			{
				d1 = 0;
			}
			if (tiles.checkSquareArea(x - 0.0625f, y) == false)
			{
				d2 = 0;
			}
			if (tiles.checkSquareArea(x, y + 0.0689655f) == false)
			{
				d3 = 0;
			}
			if (tiles.checkSquareArea(x, y - 0.0689655f) == false)
			{
				d4 = 0;
			}

			if ((d1 > d2) & (d1 > d3) & (d1 > d4))
			{
				destX = x + 0.0625f;
				destY = y;
			}
			else if ((d2 > d1) & (d2 > d3) & (d2 > d4))
			{
				destX = x - 0.0625f;
				destY = y;
			}
			else if ((d3 > d1) & (d3 > d2) & (d3 > d4))
			{
				destX = x;
				destY = y + 0.0689655f;
			}
			else if ((d4 > d1) & (d4 > d2) & (d4 > d3))
			{
				destX = x;
				destY = y - 0.0689655f;
			}

		}
		else
		{
			// Chart a course for a new destination if we've arrived at previous destination. 
			if ((fabs(x - destX) < 0.05f) & (fabs(y - destY) < 0.05f))
			{
				// Move up
				if (dir == 0)
				{
					if (y > 1.075f)
					{
						y = -1.075f;
					}
					if (tiles.checkSquareArea(x, y + 0.0689655f) == true)
					{
						destY = y + 0.0689655f;
						destX = x;
					}
					else
					{
						dir = rand() % 4;
					}
				}

				// Move right
				if (dir == 1)
				{
					if (tiles.checkSquareArea(x + 0.0625f, y) == true)
					{
						destX = x + 0.0625f;
						destY = y;
					}
					else
					{
						dir = rand() % 4;
					}
				}

				// Move down
				if (dir == 2)
				{

					if (y < -1.075f)
					{
						y = 1.075f;
					}
					if (tiles.checkSquareArea(x, y - 0.0689655f) == true)
					{
						destY = y - 0.0689655f;
						destX = x;
					}
					else
					{
						if (tiles.checkSquareArea(x, y) == true)
						{
							destY = ((std::floor(y / 0.0689655f)) *0.0689655f) + 0.01f;
							dir = -1;
						}
						dir = rand() % 4;
					}


				}

				// Move left
				if (dir == 3)
				{
					if (tiles.checkSquareArea(x - 0.0625f, y) == true)
					{
						destX = x - 0.0625f;
						destY = y;
					}
					else
					{
						dir = rand() % 4;
					}
				}
			}
		}
	}

	Renderer::SquareObj newVertices;
	newVertices.vertices[0] = { x - 0.06f, y - 0.06f, 0, type*0.3f, 1 };
	newVertices.vertices[1] = { x+0.06f, y - 0.06f, 0, (type+1)*0.3f , 1 };
	newVertices.vertices[2] = { x - 0.06f, y+0.06f, 0, type*0.3f, 0 };
	newVertices.vertices[3] = { x+0.06f, y+0.06f, 0, (type + 1)*0.3f, 0 };

	return newVertices;
}