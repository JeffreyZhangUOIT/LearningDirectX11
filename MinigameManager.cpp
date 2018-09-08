#include "MinigameManager.h"

MinigameManager::MinigameManager(bool online) 
{
	active = online;
	game = -1;
	gameover = false;
}

MinigameManager::~MinigameManager() 
{

}

void MinigameManager::init(char parameters[1000])
{
	gameover = false;
	int i = 0;
	memset(minigamename, '\0', sizeof(minigamename));
	memset(scoreBuf, '\0', sizeof(scoreBuf));
	memset(sceneBuf, '\0', sizeof(sceneBuf));
	memset(quote1, '\0', sizeof(quote1));
	memset(quote2, '\0', sizeof(quote2));
	memset(quote3, '\0', sizeof(quote3));
	// Convert parameters into minigamename, the score buffer, and scene buffer.
	token = strtok_s(parameters, "{} ", &nextTok);
	while (*token)
	{
		minigamename[i] = *token;
		token++;
		i++;
	}
	i = 0;
	token = strtok_s(NULL, "{} ", &nextTok);
	while (*token)
	{
		scoreBuf[i] = *token;
		token++;
		i++;
	}
	i = 0;
	token = strtok_s(NULL, "{} ", &nextTok);
	while (*token)
	{
		sceneBuf[i] = *token;
		token++;
		i++;
	}
	i = 0;

	// Use special minigame rules
	specialObjectives = false;
	
	// Special case. Read from minigame file.
	specialObjectives = true;
	quoteManager();

	flag1 = false;
	flag2 = false;
	flag3 = false;
	game = minigamename[1] - '0';
	active = true;
	progress = 0;
}

void MinigameManager::update(Timer& timer, MinigameMetadata& miniData, TextHandler& textHandle)
{
	// Housekeeping
	deltaDT = timer.deltaFT() - deltaDT;
	if (deltaDT < 0.001f) {
		deltaDT = 0.001f;
	}
	playerScore = miniData.minimeta.score;

	// If this minigame has quotes in the middle of it. Error, not working ATM.
	if (specialObjectives) 
	{
		if (game == 1) 
		{
			// display quotes when objectives are cleared.
			if ((!flag1) & (miniData.minimeta.kills > 1 * miniData.minimeta.difficulty)) {
				// Killed 3*difficulty.

				wchar_t * buffer1;
				int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote1, -1, NULL, 0);
				buffer1 = new wchar_t[size];
				if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote1, -1, buffer1, size)) {
					exit(99);
				}
				textHandle.tempMessage(500, 0, -0.8f, buffer1, red);
				flag1 = true;
			}

			if ((!flag2) & (miniData.minimeta.kills > 5 * miniData.minimeta.difficulty)) {
				
				wchar_t * buffer1;
				int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote2, -1, NULL, 0);
				buffer1 = new wchar_t[size];
				if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote2, -1, buffer1, size)) {
					exit(99);
				}
				textHandle.tempMessage(500, 0, -0.7f, buffer1, red);
				flag2 = true;
			}

			if ((!flag3) & (miniData.minimeta.elapsedTime > 10)) {
				
				wchar_t * buffer1;
				int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote3, -1, NULL, 0);
				buffer1 = new wchar_t[size];
				if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote3, -1, buffer1, size)) {
					exit(99);
				}
				textHandle.tempMessage(500, 0, -0.6f, buffer1, { 1, 0, 0, 1 });
				flag3 = true;
			}
		}

		if (game == 2)
		{
			// display quotes when objectives are cleared.
			if ((!flag1) & (miniData.minimeta.score > miniData.minimeta.difficulty * 50)) {
				
				wchar_t * buffer1;
				int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote1, -1, NULL, 0);
				buffer1 = new wchar_t[size];
				if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote1, -1, buffer1, size)) {
					exit(99);
				}
				textHandle.tempMessage(500, 0, -0.8f, buffer1, red);
				flag1 = true;
			}

			if ((!flag2) & (miniData.minimeta.score > miniData.minimeta.difficulty * 100)) {
				
				wchar_t * buffer1;
				int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote2, -1, NULL, 0);
				buffer1 = new wchar_t[size];
				if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote2, -1, buffer1, size)) {
					exit(99);
				}
				textHandle.tempMessage(500, 0, -0.7f, buffer1, red);
				flag2 = true;
			}

			if ((!flag3) & (miniData.minimeta.score > miniData.minimeta.difficulty * 150)) {

	wchar_t * buffer1;
	int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote3, -1, NULL, 0);
	buffer1 = new wchar_t[size];
	if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote3, -1, buffer1, size)) {
		exit(99);
	}
	textHandle.tempMessage(500, 0, -0.6f, buffer1, { 1, 0, 0, 1 });
	flag3 = true;
}
		}

		if (game == 3)
		{
			// display quotes when objectives are cleared.
			if ((!flag1) & (miniData.minimeta.distanceTraveled > miniData.minimeta.difficulty * 10)) {
				// Killed 10.
				wchar_t * buffer1;
				int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote1, -1, NULL, 0);
				buffer1 = new wchar_t[size];
				if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote1, -1, buffer1, size)) {
					exit(99);
				}
				textHandle.tempMessage(500, 0, -0.8f, buffer1, red);
				flag1 = true;
			}

			if ((!flag2) & (miniData.minimeta.distanceTraveled > miniData.minimeta.difficulty * 20)) {
				// Killed 10.
				wchar_t * buffer1;
				int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote2, -1, NULL, 0);
				buffer1 = new wchar_t[size];
				if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote2, -1, buffer1, size)) {
					exit(99);
				}
				textHandle.tempMessage(500, 0, -0.7f, buffer1, red);
				flag2 = true;
			}


			if ((!flag3) & (miniData.minimeta.distanceTraveled > miniData.minimeta.difficulty * 40)) {
				// Killed 10.
				wchar_t * buffer1;
				int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote3, -1, NULL, 0);
				buffer1 = new wchar_t[size];
				if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote3, -1, buffer1, size)) {
					exit(99);
				}
				textHandle.tempMessage(500, 0, -0.6f, buffer1, { 1, 0, 0, 1 });
				flag3 = true;
			}
		}

		if (game == 4)
		{
			// display quotes when objectives are cleared.
			if ((!flag1) & (miniData.minimeta.captures > miniData.minimeta.difficulty * 1)) {

				wchar_t * buffer1;
				int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote1, -1, NULL, 0);
				buffer1 = new wchar_t[size];
				if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote1, -1, buffer1, size)) {
					exit(99);
				}
				textHandle.tempMessage(500, 0, -0.8f, buffer1, red);
				flag1 = true;
			}

			if ((!flag2) &(miniData.minimeta.captures > miniData.minimeta.difficulty * 2)) {

				wchar_t * buffer1;
				int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote2, -1, NULL, 0);
				buffer1 = new wchar_t[size];
				if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote2, -1, buffer1, size)) {
					exit(99);
				}
				textHandle.tempMessage(500, 0, -0.7f, buffer1, red);
				flag2 = true;
			}


			if ((!flag3) &(miniData.minimeta.captures > miniData.minimeta.difficulty * 3)) {

				wchar_t * buffer1;
				int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote3, -1, NULL, 0);
				buffer1 = new wchar_t[size];
				if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, quote3, -1, buffer1, size)) {
					exit(99);
				}
				textHandle.tempMessage(500, 0, -0.6f, buffer1, { 1, 0, 0, 1 });
				flag3 = true;
			}
		}

	}

	// Write out important information during minigames.
	
	if (game == 1)
	{
		std::string floatNum = std::to_string(miniData.minimeta.elapsedTime);
		floatNum.erase(floatNum.find_first_of('.') + 2, std::string::npos);
		
		std::string rawStatus = "Elapsed Time: " + floatNum + "s HP: " + std::to_string(miniData.minimeta.playerHp);
		int n = rawStatus.length();
		char statusArr[1000];
		strcpy_s(statusArr, rawStatus.c_str());

		wchar_t * status;
		int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, statusArr, -1, NULL, 0);
		status = new wchar_t[size];
		if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, statusArr, -1, status, size)) {
			exit(99);
		}
		textHandle.drawMyText(status, -0.7f, 0.9f, { 1, 1, 1, 1 });
		
	}

	if (game == 2)
	{
		std::string floatNum = std::to_string(miniData.minimeta.timeLimit - miniData.minimeta.elapsedTime);
		floatNum.erase(floatNum.find_first_of('.') + 2, std::string::npos);

		std::string rawStatus = "Remaining Time: " + floatNum + "s Remaining Moves: " + std::to_string((60 / miniData.minimeta.difficulty) - miniData.minimeta.moves);
		int n = rawStatus.length();
		char statusArr[1000];
		strcpy_s(statusArr, rawStatus.c_str());

		wchar_t * status;
		int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, statusArr, -1, NULL, 0);
		status = new wchar_t[size];
		if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, statusArr, -1, status, size)) {
			exit(99);
		}
		textHandle.drawMyText(status, -0.7f, 0.9f, { 1, 1, 1, 1 });
		
	}

	if (game == 3)
	{
		std::string floatNum = std::to_string(miniData.minimeta.timeLimit - miniData.minimeta.elapsedTime);
		floatNum.erase(floatNum.find_first_of('.') + 2, std::string::npos);

		std::string rawStatus = "Remaining Time: " + floatNum + "s";
		int n = rawStatus.length();
		char statusArr[1000];
		strcpy_s(statusArr, rawStatus.c_str());

		wchar_t * status;
		int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, statusArr, -1, NULL, 0);
		status = new wchar_t[size];
		if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, statusArr, -1, status, size)) {
			exit(99);
		}
		textHandle.drawMyText(status, -0.7f, 0.9f, { 1, 1, 1, 1 });
		
	}
	
	if (game == 4)
	{
		std::string floatNum = std::to_string(miniData.minimeta.timeLimit - miniData.minimeta.elapsedTime);
		floatNum.erase(floatNum.find_first_of('.') + 2, std::string::npos);

		std::string rawStatus = "Remaining Time: " + floatNum + "s";
		int n = rawStatus.length();
		char statusArr[1000];
		strcpy_s(statusArr, rawStatus.c_str());

		wchar_t * status;
		int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, statusArr, -1, NULL, 0);
		status = new wchar_t[size];
		if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, statusArr, -1, status, size)) {
			exit(99);
		}
		textHandle.drawMyText(status, -0.7f, 0.9f, { 1, 1, 1, 1 });
		
	}

	// Determine if special settings have concluded the minigame, and then start the appropriate scene.
	if (active) {
		if (miniData.minimeta.elapsedTime > miniData.minimeta.timeLimit) {
			// Ran out of time
			if (game == 2)
			{
				playerScore = miniData.minimeta.score;
				gameover = true;
			}

			if (game == 3) 
			{
				miniData.minimeta.score = miniData.minimeta.distanceTraveled;
				playerScore = miniData.minimeta.score;
				gameover = true;	
			}

			if (game == 4)
			{
				miniData.minimeta.score = miniData.minimeta.captures;
				playerScore = miniData.minimeta.score;
				gameover = true;
			}
		}

		// Game 1 Win Condition
		if (game == 1) {
			if (miniData.minimeta.kills > 10 * miniData.minimeta.difficulty) {
				miniData.minimeta.score = miniData.minimeta.kills;
				playerScore = miniData.minimeta.score;
				gameover = true;
			}
			
		}

		// Game 2 win conditions
		if (game == 2) {
			if (miniData.minimeta.moves >= (60 / miniData.minimeta.difficulty) ) 
			{
				// Run out of moves
				playerScore = miniData.minimeta.score;
				gameover = true;
			}
			if (miniData.minimeta.score >= 1500) 
			{
				// Got enough score
				playerScore = miniData.minimeta.score;
				gameover = true;
			}
		}

		// Game 3 win conditions
		if (game == 3) 
		{
			if (miniData.minimeta.distanceTraveled > miniData.minimeta.difficulty * 50)
			{
				miniData.minimeta.score = miniData.minimeta.distanceTraveled;
				playerScore = miniData.minimeta.score;
				gameover = true;
			}
		}

		// Game 4 win conditions
		if (game == 4)
		{
			// Sent all students to class
			if (miniData.minimeta.captures >= miniData.minimeta.difficulty * 4) 
			{
				miniData.minimeta.score = miniData.minimeta.captures;
				playerScore = miniData.minimeta.score;
				gameover = true;
			}
		}

	}

	// Player died.
	if (miniData.minimeta.playerHp <= 0) {
		playerScore = miniData.minimeta.score;
		gameover = true;
	}
	
}

bool MinigameManager::gameState() 
{
	return gameover;
}

char * MinigameManager::setScene()
{
	float scoreDelim = -2;
	int counter = 0;
	int i = 0;
	
	token = strtok_s(scoreBuf, ", ", &nextTok);
	scoreDelim = std::stof(token);
	
	while (scoreDelim < playerScore) {
		token = strtok_s(NULL, ", ", &nextTok);
		while (token != NULL) {
			scoreDelim += std::stof(token);
			counter++;
			token = strtok_s(NULL, ", ", &nextTok);
		}
		if (token == NULL) {
			counter++;
			break;
		}
	}

	token = strtok_s(sceneBuf, ", ", &nextTok);
	while (counter > 0) {
		token = strtok_s(NULL, ", ", &nextTok);
		counter--;
	}

	return token;
}

void MinigameManager::quoteManager() 
{
	char buffer[1000];

	fopen_s(&quotes, "MinigameQuotes.txt", "r+");
	if (quotes) {
		while (fgets(buffer, sizeof(buffer), quotes) != NULL) {
			token = strtok_s(buffer, "/[]\n", &nextTok);
			if (token)
			{
				if (strncmp(token, minigamename, strlen(token)) == 0)
				{
					// Found the minigame.
					fgets(quote1, sizeof(quote1), quotes);
					fgets(quote2, sizeof(quote2), quotes);
					fgets(quote3, sizeof(quote3), quotes);
					break;
				}
			}

			//strncpy_s(parameters, 1000, token, 999);
		}
	}
	else {
		// Cannot open MinigameQuotes.txt
		exit(404);
	}

	if (quotes) {
		fclose(quotes);
	}
}