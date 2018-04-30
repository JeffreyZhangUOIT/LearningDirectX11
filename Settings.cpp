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

#include "Settings.h"
using namespace std;

Settings::Settings(SoundManager& music) {
	fopen_s(&settings, "settings.txt", "r+");
	if (settings == NULL) {
		fopen_s(&settings, "settings.txt", "w+");
	}
	if (settings != NULL) {
		char buffer[256];
		char * token = NULL;
		char * nextTok = NULL;

		while (fgets(buffer, 256, settings) != NULL) {

			token = strtok_s(buffer, "=\n", &nextTok);
	
			if (strncmp(token, "MasterVolume", strlen("MasterVolume")) == 0) {
				token = strtok_s(NULL, "=\n", &nextTok);
				masterVolume = stof(token);
			}
			if (strncmp(token, "SomeOtherParam", strlen("SomeOtherParam")) == 0) {
				token = strtok_s(NULL, "=\n", &nextTok);
				someOtherParam = stof(token);
			}
		}
	}
	if (!settings) {
		//can't open file error. send user a prompt.
	}
	if (settings) {
		fclose(settings);
	}
	
	update(music, masterVolume);
}

Settings::~Settings() {
	if (settings) {
		fclose(settings);
	}
	
}

void Settings::update(SoundManager& music, float vol) {
	masterVolume = vol;
	music.setVolume(masterVolume);
}

void Settings::save(SoundManager& music) {
	fopen_s(&settings, "settings.txt", "w+");
	char buf[256];
	char temp[256];

	fprintf(settings, "[Game Settings]\n", buf);

	strcpy_s(buf, sizeof(buf), "MasterVolume=");
	sprintf_s(temp, sizeof(temp), "%.2f", masterVolume);
	strcat_s(buf, 256, temp);
	fprintf(settings, "%s\n", buf);

	strcpy_s(buf, sizeof(buf), "SomeOtherParam=72");
	fprintf(settings, "%s\n", buf);
	fclose(settings);
	//strcat_s(buf, temp);
	

	//update(music, masterVolume);
}