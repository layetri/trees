//
// Created by Daniël Kamp on 22/06/2021.
//

#include "Header/Analyzer.h"

Analyzer::Analyzer(int size, Buffer* input) {
  buffer = input;
  fft_size = size;
}

Analyzer::~Analyzer() {

}

void Analyzer::process() {
  //Complex data = buffer->getPastNSamples(fft_size);
  CArray chunk[fft_size];
  //fft(chunk);

  // Do FFT analysis
  for(int i = 0; i < fft_size; i++) {
    chunk[i] = buffer->readAhead(i);
  }

  // Calculate variables for spatialisation

  // Apply filtering

}

CArray Analyzer::fft(CArray& chunk) {
  // Cooley–Tukey FFT (in-place, divide-and-conquer)
  // Borrowed from https://rosettacode.org/wiki/Fast_Fourier_transform#C.2B.2B

  CArray rtn_data[fft_size];

  size_t N = chunk.size();
  if (N <= 1) return chunk;

  // divide
  CArray even = chunk[std::slice(0, N/2, 2)];
  CArray odd = chunk[std::slice(1, N/2, 2)];

  // conquer
  even = fft(even);
  odd = fft(odd);

  // combine
  for (size_t k = 0; k < N/2; ++k)
  {
    Complex t = std::polar(1.0, -2 * M_PI * k / N) * odd[k];
    rtn_data[k] = even[k] + t;
    rtn_data[k+N/2] = even[k] - t;
  }

  return *rtn_data;
}
