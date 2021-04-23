#include "Header/Pan.h"
#define TWO_PI (2 * M_PI)

Pan::Pan()
{     
}

Pan::~Pan()
{
}

void Pan::setangleDeg(float angleDeg)
{
	this -> angleDeg = angleDeg;
}

void Pan::setpanning(float panning)
{
	this -> panning = panning;
}

float Pan::getangleDeg()
{
	return angleDeg;
}

float Pan::getpanning()
{
	return panning;
}

float Pan::gainCalL(float angleDeg, float panning)
{
	float angle = angleDeg / 360.;
	float anglePan = panning * (tan(TWO_PI * angle));
	float afstand = (1 / cos(TWO_PI * angle));

	float a = sqrt(anglePan * anglePan + 1);
	float b = atan(anglePan)/TWO_PI;

	float gainL = (cos((b+angle)*TWO_PI)*afstand/a*2/(afstand+1));
	std::cout << "L: " << gainL << std::endl;
	return gainL;
}

float Pan::gainCalR(float angleDeg, float panning)
{
	float angle = angleDeg / 360.;
	float anglePan = panning * (tan(TWO_PI * angle));
	float afstand = (1 / cos(TWO_PI * angle));

	float a = sqrt(anglePan * anglePan + 1);
	float b = atan(anglePan)/TWO_PI;

	float gainR = (cos((b-angle)*TWO_PI)*afstand/a*2/(afstand+1));
	std::cout << "R: " << gainR << std::endl;
	return gainR;
}