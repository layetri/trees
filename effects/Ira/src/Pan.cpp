#include "Pan.h"
#include "jack_module.h"
#include "math.h"

Pan::Pan(int angleDeg, float panning)
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
	float anglePan = panning * (tan(PI_2 * angle));
	float afstand = (1 / cos(PI_2 * angle));

	float a = sqrt(anglePan * anglePan + 1);
	float b = atan(anglePan)/PI_2; 

	float gainL = (cos((b+angle)*PI_2)*afstand/a*2/(afstand+1));
	std::cout << "L: " << gainL << std::endl;
	return gainL;
}

float Pan::gainCalR(float angleDeg, float panning)
{
	float angle = angleDeg / 360.;
	float anglePan = panning * (tan(PI_2 * angle));
	float afstand = (1 / cos(PI_2 * angle));

	float a = sqrt(anglePan * anglePan + 1);
	float b = atan(anglePan)/PI_2; 

	float gainR = (cos((b-angle)*PI_2)*afstand/a*2/(afstand+1));
	std::cout << "R: " << gainR << std::endl;
	return gainR;
}