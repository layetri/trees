#ifndef _PAN_H_
#define _PAN_H_

#include <iostream>
#include <math.h>
#include "Global.h"

#if defined(PLATFORM_TEENSY_40)
  #include <Arduino.h>
#elif defined(PLATFORM_DARWIN_X86)
  #include <cmath>
  #include <cstdint>
#endif

class Pan
{
public:
	Pan();
	~Pan();

//setters
	void setangleDeg(float angleDeg);
	void setpanning(float panning);
//getters
	float getangleDeg();
	float getpanning();

	float* getSample();
	void tick();
//Methods
	float gainCalL(float angleDeg, float panning);
	float gainCalR(float angleDeg, float panning);
protected:
	float angle;
	float afstand;
	float a;
	float b;
	float angleDeg;
	float panning;
	float gainL;
	float gainR;
};

#endif