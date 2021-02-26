//
// Created by Daniël Kamp on 25/02/2021.
//

#ifndef _SNOWSTORM_REVERB_H
#define _SNOWSTORM_REVERB_H

#include "DelayLine.h"
#include "LowPassFilter.h"

class Reverb {
  public:
    Reverb(float tail, int samplerate, Buffer *input, Buffer *output);
    ~Reverb();

    void tick();
    void process();

    void setTail(float length);
  private:
    void calculateRatios();

    Buffer *input;
    Buffer *output;
    Buffer *y;

    LowPassFilter *lpf;

    DelayLine *dl1;
    DelayLine *dl2;
    DelayLine *dl3;
    DelayLine *dl4;

    float tail;
    float sample;

    // Gain ratios
    float n1, n2, n3, n4;

    // Delay times, feedback ratios
    float fb1, fb2, fb3, fb4;
    int dt1, dt2, dt3, dt4;
};


#endif //_SNOWSTORM_REVERB_H
