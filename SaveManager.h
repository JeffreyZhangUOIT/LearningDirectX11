#pragma once

#include <string>
#include <stdio.h> 
#include "SoundManager.h"
#include "TextGrabber.h"
#include "PoseArtManager.h"
#include "BackgroundManager.h"
#include "TransitionManager.h"
#include "Timer.h"

class SaveManager {
public:
	struct saveStruct {
		int chapter;
		int offset;
	};
	SaveManager(int x);
	~SaveManager();
	void Save(TextGrabber& storyReader, TextHandler& text, PoseArtManager& pose, Background& bg);
	void Load(TextGrabber& storyReader, TextHandler& text, PoseArtManager& pose, Transition& transistor, Timer& time);
private:
	
	FILE* save;
	saveStruct currentSave;
	// Any other important flags go here.
};