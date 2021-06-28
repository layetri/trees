//
// Created by Daniël Kamp on 23/06/2021.
//

#ifndef CAVES_SPATIAL_OBJECT_H
#define CAVES_SPATIAL_OBJECT_H

// Include global dependencies
#include "Global.h"

// Include project dependencies
#include "Buffer.h"
#include "Analyzer.h"
#include <iostream>

// Include platform dependencies
#if defined(PLATFORM_TEENSY_40)
  #include <Arduino.h>
#elif defined(PLATFORM_DARWIN_X86)
  #include <cmath>
  #include <cstdint>
#endif

class SpatialObject {
  public:
    SpatialObject(Buffer* input);
    ~SpatialObject();

    int16_t* processOutputSample();

  private:
    void calcGain();

    // Define variables
    Buffer* input_buffer;
    Analyzer* analyzer;

    int16_t sample_left, sample_right;
    int16_t sample[2];
    float* location;
    float gain_left, gain_right;
    int cnt;
};


#endif //CAVES_SPATIAL_OBJECT_H
