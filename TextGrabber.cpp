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

#include "TextGrabber.h"

TextGrabber::TextGrabber(int ch) {
	chapter = ch;
	offset = 0;
	command[256] = NULL;
	parameters[1000] = NULL;
	loadTheChapter();
	firstInstance = true;
}

TextGrabber::~TextGrabber() {
	if (fp) {
		fclose(fp);
	}
}

void TextGrabber::getNextCommand() {
	firstInstance = true;
	fopen_s(&fp, "Default.txt", "w+");
	if (chapter == 1) {
		fopen_s(&fp, "Chapter1.txt", "r");
	}
	if (chapter == 2) {
		fopen_s(&fp, "Chapter2.txt", "r");
	}
	if (fp) {
		fseek(fp, indexedLines[offset], 0);

		if (fgets(buffer, sizeof(buffer), fp) != NULL) {
			token = strtok_s(buffer, "/:[]\n", &nextTok);
			if (token)
			{
				memset(command, '\0', sizeof(command));
				memset(parameters, '\0', sizeof(parameters));
				int pos = 0;
				while (*token)
				{
					command[pos] = *token;
					token++;
					pos++;
				}
				//strncpy_s(command, 256, token, 255);
				token = strtok_s(NULL, "/:[]\n", &nextTok);
				if (token)
				{
					pos = 0;
					while (*token)
					{
						parameters[pos] = *token;
						token++;
						pos++;
					}
				}
			}

			
			//strncpy_s(parameters, 1000, token, 999);
		}
		else {
			// Somehow, we still reached end of file.
		}

		if (offset < eofPos) {
			offset++;
		}
		else {
			// We have reached end of file.
		}
		fclose(fp);
	}
}

int TextGrabber::execCommand(SoundManager& sounds, TextHandler& drawText) {
	fopen_s(&fp, "Default.txt", "w+");
	if (chapter == 1) {
		fopen_s(&fp, "Chapter1.txt", "r");
	}
	if (chapter == 2) {
		fopen_s(&fp, "Chapter2.txt", "r");
	}

	if (strncmp(command, "SCENE", strlen("SCENE")) == 0) {
		// Beginning of a scene, just advance to next command.
	
		getNextCommand();
	}
	else if (strcmp(command, "NextScene") == 0) {
		// Curate parameters for undesirable input.
		int j = 0;
		for (int i = 0; i < sizeof(parameters); i++) {
			if (parameters[i] == ' ') {
				i++;
			}
			parameters[j] = parameters[i];
			j++;
		}
		// Premature optimization.
		//int n = std::stoi(strtok_s(parameters,"SCENABCD\n", &nextTok));
		//fseek(fp, indexedScenes[n], 0);

		offset = 0;
		char temp[1000];
		strcpy_s(temp, "[");
		//strcat_s(parameters, sizeof(parameters), "\n");
		strcat_s(temp, sizeof(temp), parameters);
		strcat_s(temp, sizeof(temp), "]");

		while (fgets(buffer, sizeof(buffer), fp) != NULL) {

			if (strncmp(buffer, temp, strlen(temp)) == 0) {
				break;
			}
			offset++;
		}
		getNextCommand();
	}
	else if (strcmp(command, "Minigame") == 0) {
		float scoreArr[10];
		int i = 0;
		char score[256];
		char minigamename[256];
		char sceneBuf[256];

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
			score[i] = *token;
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
		// Run appropriate minigame with score and scenebuf as input.
		if (fp) {
			fclose(fp);
		}

		return 3;
	}

	else if (strcmp(command, "Transition") == 0) {
		// send command to transition class with paramters as parameters.
		
		getNextCommand();
	}

	else if (strcmp(command, "Chapter") == 0) {
		//Weird text glitch from following line. Dunno why.
		char temp[1000];
		memcpy_s(temp, sizeof(temp), parameters, sizeof(parameters));
		setChOff(std::stoi(temp), 0);
		getNextCommand();
	}
	else if (strcmp(command, "SFX") == 0) {
		// send command to SoundManager class

		getNextCommand();
	}
	else if (strcmp(command, "BGM") == 0) {
		// send command to SoundManager class

		getNextCommand();
	}
	else if (strcmp(command, "Exit") == 0) {
		// send command to poseart manager

		getNextCommand();
	}
	else if (strcmp(command, "Quit") == 0) {
		if (fp) {
			fclose(fp);
		}
		return 0;
	}
	else if (strcmp(command, "Enter") == 0) {
		// send command to poseart manager
		
	}

	else {
		// It's a character talking. Send name to poseart manager.
		if (firstInstance) {
			drawText.resetMyTime();
			firstInstance = false;
		}
		drawText.drawDialog(command, parameters, red);
	}
	if (fp) {
		fclose(fp);
	}
	return 2;
	
}

void TextGrabber::setChOff(int ch, int off) 
{
	offset = off;
	chapter = ch;
	loadTheChapter();
}

void TextGrabber::minigameReturn(char* nextScene) 
{
	strcpy_s(command, "NextScene");
	strcpy_s(parameters, nextScene);
}

void TextGrabber::loadTheChapter() 
{
	fopen_s(&fp, "Default.txt", "w+");

	if (chapter == 1) {
		fopen_s(&fp, "Chapter1.txt", "r");
	}
	if (chapter == 2) {
		fopen_s(&fp, "Chapter2.txt", "r");
	}

	indexedLines[0] = 0;
	indexedScenes[0] = 0;
	int i = 1;
	int j = 1;
	while (fgets(buffer, 1256, fp) != NULL) {
		indexedLines[i] = ftell(fp);
		if (strncmp(buffer, "[SCENE", strlen("[SCENE")) == 0) {
			indexedScenes[j] = ftell(fp);
			j++;
		}
		i++;
	}
	eofPos = i;
	fclose(fp);
}