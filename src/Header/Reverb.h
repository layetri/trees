//
// Created by Daniël Kamp on 25/02/2021.
//

#ifndef _SNOWSTORM_REVERB_H
#define _SNOWSTORM_REVERB_H

#include "DelayLine.h"
#include "LowPassFilter.h"
#include "Global.h"
#include <vector>

#if defined(PLATFORM_TEENSY_40)
  #include <Arduino.h>
#elif defined(PLATFORM_DARWIN_X86)
  #include <cmath>
  #include <cstdint>
#endif

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

    std::vector<DelayLine*> dl;

    int dl_size;
    int samplerate;
    float tail;
    int16_t sample;
};


#endif //_SNOWSTORM_REVERB_H
