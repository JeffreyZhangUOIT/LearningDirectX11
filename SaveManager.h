#pragma once

#include <string>
#include <stdio.h> 
#include "SoundManager.h"
#include "TextGrabber.h"

class SaveManager {
public:
	struct saveStruct {
		int chapter;
		int offset;
	};
	SaveManager(int x);
	~SaveManager();
	void Save(TextGrabber& storyReader, TextHandler& text);
	void Load(TextGrabber& storyReader, TextHandler& text);
private:
	FILE* save;
	saveStruct currentSave;
	// Any other important flags go here.
};