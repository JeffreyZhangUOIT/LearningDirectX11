#include "Timer.h"


void Timer::resetTime() {
	startTime = clock();
}

unsigned long Timer::elapsedTime() {
	return ((unsigned long)clock() - startTime) / CLOCKS_PER_SEC;
}
