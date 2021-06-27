//
// Created by Daniël Kamp on 22/06/2021.
//

#ifndef CAVES_ANALYZER_H
#define CAVES_ANALYZER_H

#include <cmath>

#include "Buffer.h"
#include "Global.h"
#include <iostream>

class Analyzer {
  public:
    Analyzer(int fft_size, Buffer* input);
    ~Analyzer();

    void process();
    float* getFloatValues();
    void fft(CArray& chunk);

  private:
    Buffer* buffer;

    double x, z;
    int fft_size, fft2;
    float f_inc;
    float xz[2] {0,0};
};


#endif //CAVES_ANALYZER_H
