//
// Created by Daniël Kamp on 22/06/2021.
//

#ifndef CAVES_ANALYZER_H
#define CAVES_ANALYZER_H

#include <complex>
#include <valarray>
#include <cmath>

#include "Buffer.h"

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

class Analyzer {
  public:
    Analyzer(int fft_size, Buffer* input);
    ~Analyzer();

    void process();
    CArray fft(CArray& chunk);

  private:
    Buffer* buffer;

    int fft_size;
};


#endif //CAVES_ANALYZER_H
