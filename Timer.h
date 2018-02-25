#pragma once
#include <time.h>

class Timer {
public:
	void resetTime();
	unsigned long elapsedTime();
private:
	unsigned long startTime;
	unsigned long deltaTime;
};