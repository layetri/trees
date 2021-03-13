#ifndef _PAN_H_
#define _PAN_H_

#include <iostream>

class Pan
{
public:
	Pan(int angleDeg, float pan);
	~Pan();

//setters
	void setangleDeg(float angleDeg);
	void setpanning(float panning);
//getters
	float getangleDeg();
	float getpanning();
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