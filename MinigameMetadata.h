#pragma once

class MinigameMetadata {
public:
	struct metadata {
		// General
		int difficulty;
		int playerHp;
		float elapsedTime;
		float timeLimit;
		float score;

		// Minigame 1: TDS
		int kills;
		int shots;

		// Minigame 2: Driving
		float distanceTraveled;
		
		// Minigame 3: Match 3
		int moves;
		float match5Plus;
		int captures;

	};
	metadata minimeta;

	MinigameMetadata() 
	{
		minimeta = { 1, 3, 0, 60000, 0, 0, 0, 0, 0, 0, 0 };
	}
	~MinigameMetadata() {}
	

	

};