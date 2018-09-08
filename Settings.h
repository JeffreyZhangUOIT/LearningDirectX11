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
#include <string>
#include <stdio.h> 
#include "SoundManager.h"
#include "TextHandler.h"

class Settings {
public:
	Settings(int i);

	~Settings();
	void init(SoundManager& music, TextHandler& text);
	void updateMasterVol(SoundManager& music, float vol);
	void updateCharPerSec(TextHandler& text, double cps);
	void save(SoundManager& music);
	char resolution[50];

private:
	bool opened = false;
	float masterVolume = 0.5f;
	float someOtherParam = 3;
	double charPerSec = 50;
	int sW, sH;
	
	FILE * settings;
};

/*
Some future settings: resolution, rebindable keys, text scaling. (other parts of UI already scales well with resolution).

*/

