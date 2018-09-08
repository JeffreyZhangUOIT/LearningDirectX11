#pragma once

#include "Timer.h"
#include "MinigameMetadata.h"
#include "TextHandler.h"
#include <cstring>
#include <string>

class MinigameManager {

public:
	MinigameManager(bool online);
	~MinigameManager();
	void update(Timer& timer, MinigameMetadata& miniData, TextHandler& textHandle);
	void init(char parameters[1000]);
	char * setScene();
	int game;
	bool gameState();
	float progress;

private:
	void quoteManager();
	float playerScore;
	bool active, specialObjectives, gameover;
	bool flag1, flag2, flag3;
	float deltaDT;
	char param[1000];
	char scoreBuf[256];
	char minigamename[256];
	char sceneBuf[256];
	char quote1[256];
	char quote2[256];
	char quote3[256];
	char bonusQuote[256];
	char * token;
	char * nextTok;
	FILE * quotes;
	DirectX::XMVECTORF32 red = { 1, 0, 0, 1 };
};