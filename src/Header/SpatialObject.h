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

// Include platform dependencies
#if defined(PLATFORM_TEENSY_40)
  #include <Arduino.h>
#elif defined(PLATFORM_DARWIN_X86)
  #include <cmath>
  #include <cstdint>
#endif

class SpatialObject {
  public:
    SpatialObject(Buffer* input, Buffer* left, Buffer* right);
    ~SpatialObject();

    void processOutputSample();

    void calcGain();
    void calcGainLeft();
    void calcGainRight();

  private:
    // Define variables
    Buffer* input_buffer;
    Buffer* output_left;
    Buffer* output_right;
    Analyzer* analyzer;

    Complex location;
    float gain_left;
    float gain_right;
};


#endif //CAVES_SPATIAL_OBJECT_H
