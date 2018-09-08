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

int TextGrabber::execCommand(SoundManager& sounds, TextHandler& drawText, MinigameManager& miniManager, Choice& choice, PoseArtManager& pose, Background& backdrop, Transition& transistor, Timer& time) {
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
	else if (strcmp(command, "Minigame") == 0) 
	{
		
		// Run appropriate minigame with score and scenebuf as input.
		miniManager.init(parameters);
		if (fp) {
			fclose(fp);
		}
		return 3;
	}

	else if (strcmp(command, "Brownie") == 0)
	{
		
		token = strtok_s(parameters, " ", &nextTok);
		if (strncmp(token, "Mifune", strlen("Mifune")) == 0)
		{
			token = strtok_s(NULL, " ", &nextTok);
			Mifune += std::stoi(token);
		}

		if (strncmp(token, "Inori", strlen("Inori")) == 0)
		{
			token = strtok_s(NULL, " ", &nextTok);
			Inori += std::stoi(token);
		}

		if (strncmp(token, "Tojihime", strlen("Tojihime")) == 0)
		{
			token = strtok_s(NULL, " ", &nextTok);
			Tojihime += std::stoi(token);
		}

		if (strncmp(token, "Akari", strlen("Akari")) == 0)
		{
			token = strtok_s(NULL, " ", &nextTok);
			Akari += std::stoi(token);
		}
		
		
	}

	else if (strcmp(command, "Transition") == 0) {
		// send command to transition class with paramters as parameters.
		token = strtok_s(parameters, " :", &nextTok);
		if (strncmp(token, "FadeToBlack", sizeof("FadeToBlack")) == 0)
		{
			token = strtok_s(NULL, " :", &nextTok);
			if (strncmp(token, "Hallway", sizeof("Hallway")) == 0)
			{
				transistor.init(0, time);
			}
			if (strncmp(token, "Home", sizeof("Home")) == 0)
			{
				transistor.init(1, time);
			}
			if (strncmp(token, "OutsideSchool", sizeof("OutsideSchool")) == 0)
			{
				transistor.init(2, time);
			}
			if (strncmp(token, "Office", sizeof("Office")) == 0)
			{
				transistor.init(3, time);
			}
			if (strncmp(token, "Lockers", sizeof("Lockers")) == 0)
			{
				transistor.init(4, time);
			}
			if (strncmp(token, "OfficeVP", sizeof("OfficeVP")) == 0)
			{
				transistor.init(5, time);
			}
			if (strncmp(token, "TitleScreen", sizeof("TitleScreen")) == 0)
			{
				transistor.init(6, time);
			}
			if (strncmp(token, "Settings", sizeof("Settings")) == 0)
			{
				transistor.init(7, time);
			}
		}

		if (strncmp(token, "None", sizeof("None")) == 0)
		{
			token = strtok_s(NULL, " :", &nextTok);
			if (strncmp(token, "Hallway", sizeof("Hallway")) == 0)
			{
				backdrop.setBackground(0);
			}
			if (strncmp(token, "Home", sizeof("Home")) == 0)
			{
				backdrop.setBackground(1);
			}
			if (strncmp(token, "OutsideSchool", sizeof("OutsideSchool")) == 0)
			{
				backdrop.setBackground(2);
			}
			if (strncmp(token, "Office", sizeof("Office")) == 0)
			{
				backdrop.setBackground(3);
			}
			if (strncmp(token, "Lockers", sizeof("Lockers")) == 0)
			{
				backdrop.setBackground(4);
			}
			if (strncmp(token, "OfficeVP", sizeof("OfficeVP")) == 0)
			{
				backdrop.setBackground(5);
			}
			if (strncmp(token, "TitleScreen", sizeof("TitleScreen")) == 0)
			{
				backdrop.setBackground(6);
			}
			if (strncmp(token, "Settings", sizeof("Settings")) == 0)
			{
				backdrop.setBackground(7);
			}
		}
		getNextCommand();
	}

	else if (strcmp(command, "Choose") == 0) 
	{
		// Send parameters to choice.h
		choice.init(parameters);
		return 4;
	}
	else if (strcmp(command, "Chapter") == 0) {
		
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
	else if (strcmp(command, "Exit") == 0) 
	{
		// 00 = Toji, 10 = Mifune, 20 = Inori, 30 = Akari, 40 = Emilia, 50 = ???, 60 = Anon Faculty, 70 = Vice Principal
		token = strtok_s(parameters, " ", &nextTok);
		if (strncmp(token, "Mifune", strlen("Mifune")) == 0)
		{
			pose.exit(10);
		}
		else if (strncmp(token, "Akari", strlen("Akari")) == 0)
		{
			pose.exit(30);
		}
		else if (strncmp(token, "Inori", strlen("Inori")) == 0)
		{
			pose.exit(20);
		}
		else if (strncmp(token, "Tojihime", strlen("Tojihime")) == 0)
		{
			pose.exit(0);
		}
		else if (strncmp(token, "A.Faculty", strlen("A.Faculty")) == 0)
		{
			pose.exit(60);
		}
		else if (strncmp(token, "VP", strlen("VP")) == 0)
		{
			pose.exit(70);
		}
		getNextCommand();
	}
	else if (strcmp(command, "ExitAll") == 0)
	{
		pose.exitAll();
		getNextCommand();
	}
	else if (strcmp(command, "Quit") == 0) {
		if (fp) {
			fclose(fp);
		}
		exit(0);
	}
	else if (strcmp(command, "Enter") == 0) 
	{
		// Send command to poseart manager
		// 00 = Toji, 10 = Mifune, 20 = Inori, 30 = Akari, 40 = Emilia, 50 = ???, 60 = Anon Faculty, 70 = Vice Principal
		if (strncmp(parameters, "Mifune", strlen("Mifune")) == 0)
		{
			token = strtok_s(parameters, " ", &nextTok);
			token = strtok_s(NULL, " ", &nextTok);
			pose.enter(10);
		}
		if (strncmp(parameters, "Akari", strlen("Akari")) == 0)
		{
			token = strtok_s(parameters, " ", &nextTok);
			token = strtok_s(NULL, " ", &nextTok);
			pose.enter(30);
		}
		if (strncmp(parameters, "Inori", strlen("Inori")) == 0)
		{
			token = strtok_s(parameters, " ", &nextTok);
			token = strtok_s(NULL, " ", &nextTok);
			pose.enter(20);
		}
		if (strncmp(parameters, "Tojihime", strlen("Tojihime")) == 0)
		{
			token = strtok_s(parameters, " ", &nextTok);
			token = strtok_s(NULL, " ", &nextTok);
			pose.enter(0);
		}
		if (strncmp(parameters, "A.Faculty", strlen("A.Faculty")) == 0)
		{
			token = strtok_s(parameters, " ", &nextTok);
			token = strtok_s(NULL, " ", &nextTok);
			pose.enter(60);
		}
		if (strncmp(parameters, "VP", strlen("VP")) == 0)
		{
			token = strtok_s(parameters, " ", &nextTok);
			token = strtok_s(NULL, " ", &nextTok);
			pose.enter(70);
		}
		if (strncmp(parameters, "Emilia", strlen("Emilia")) == 0)
		{
			token = strtok_s(parameters, " ", &nextTok);
			token = strtok_s(NULL, " ", &nextTok);
			pose.enter(40);
		}
		getNextCommand();
	}

	else {

		if (firstInstance) 
		{
			// It's a character talking. Send name to poseart manager.
			// 00 = Toji, 10 = Mifune, 20 = Inori, 30 = Akari, 40 = Emilia, 50 = ???, 60 = Anon Faculty, 70 = Vice Principal
			 if (strncmp(command, "Tojihime", strlen("Tojihime")) == 0)
			{
				pose.enter(0);
			}
			else if (strncmp(command, "Mifune", strlen("Mifune")) == 0)
			{
				pose.enter(10);
			}
			else if (strncmp(command, "Inori", strlen("Inori")) == 0)
			{
				pose.enter(20);
			}
			else if (strncmp(command, "Akari", strlen("Akari")) == 0)
			{
				pose.enter(30);
			}
			else if (strncmp(command, "Emilia", strlen("Emilia")) == 0)
			{
				pose.enter(40);
			}
			else if (strncmp(command, "A.Faculty", strlen("A.Faculty")) == 0)
			{
				pose.enter(60);
			}
			else if (strncmp(command, "VP", strlen("VP")) == 0)
			{
				pose.enter(70);
			}

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