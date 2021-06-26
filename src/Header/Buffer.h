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
    Buffer(int length, std::string name);
    ~Buffer();

    void write(int16_t sample);
    void writeAhead(int16_t sample, int places);
    void writeAddition(int16_t sample);

    void tick();
    void flush();

    int getPosition();
    int getSize();
    std::string getName();

    int16_t getSample(int sample_position);
    int16_t getCurrentSample();
    int16_t readAhead(int places);
    int16_t readBack(int places);

    int16_t& operator[] (int index) {
      return data[index];
    }

private:
    int16_t *data;
    int size;
    int position;
    std::string name;

    bool full_cycle_flag;
};

#endif //SNOWSTORM_BUFFER_H
