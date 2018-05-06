/*
Copyright 2017, 2018 Jeffrey Zhang

This file is part of ProjectFiasco.

ProjectFiasco is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ProjectFiasco is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ProjectFiasco.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#include "SoundManager.h"
#include "TextHandler.h"
#include "BoxFactory.h"
#include <stdio.h> 


class TextGrabber {
public:
	TextGrabber(int ch);
	~TextGrabber();
	int chapter;
	int offset;
	void loadTheChapter();
	void minigameReturn(char* nextScene);
	void getNextCommand();
	int execCommand(SoundManager& sounds, TextHandler& drawText);
	void setChOff(int ch, int off);

private:
	FILE * fp;
	char command[256];
	char parameters[1000];
	char buffer[1256];
	char* token;
	char* nextTok;
	bool firstInstance;
	int indexedLines[3000];
	int indexedScenes[100];

	int eofPos;

	DirectX::XMVECTORF32 red = { 1, 0, 0, 1 };

};