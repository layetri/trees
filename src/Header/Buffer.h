//
// Created by Daniël Kamp on 17/02/2021.
//

#ifndef SNOWSTORM_BUFFER_H
#define SNOWSTORM_BUFFER_H

#include <string>
#include "Global.h"

#if defined(PLATFORM_TEENSY_40)
  #include <Arduino.h>
#elif defined(PLATFORM_DARWIN_X86)
  #include <cmath>
  #include <cstdint>
#endif

class Buffer {
public:
    Buffer(int length, std::string name="Generic Buffer");
    ~Buffer();

    void write(sample_t sample);
    void writeAhead(sample_t sample, int places);
    void writeAddition(sample_t sample);

    void tick();
    void flush();
    void wipe();

    int getPosition();
    int getSize();
    std::string getName();

    sample_t getSample(int sample_position);
    sample_t getCurrentSample();
    sample_t readAhead(int places);
    sample_t readBack(int places);

    sample_t& operator[] (int index) {
      return data[index];
    }

private:
    sample_t *data;
    int size;
    int position;
    std::string name;

    bool full_cycle_flag;
};

#endif //SNOWSTORM_BUFFER_H
