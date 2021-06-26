//
// Created by Daniël Kamp on 22/06/2021.
//

#include "Header/Analyzer.h"
#define TWO_PI (M_PI * 2)

Analyzer::Analyzer(int size, Buffer* input) {
  buffer = input;
  fft_size = size;
  x = 0;
  z = 0;

  verbose(input);
  verbose(buffer);
}

Analyzer::~Analyzer() {}

// ========================================
//  Analysis process
// ========================================
void Analyzer::process() {
  // TODO: at bus error, it doesn't even get to here
  //  -> investigate!!
  CArray chunk(fft_size);
  double freq_avg, amp_avg;
  amp_avg = 0.0;
  freq_avg = fft_size;

  // Do FFT analysis
  for(int i = 0; i < fft_size/2; i++) {
    // TODO: it dies here
//    verbose("starting fft");
//    verbose(buffer);

    float val = (buffer->readBack((fft_size / 2) - i) / 65536.0) - 1.0;

    amp_avg += abs(val);
    chunk[i] = val;
  }
  fft(chunk);


  // Calculate weighed average of spectrum and amplitude
  double loudest[2] = {0.0, 0.0};
//    verbose("fft done");

  for(int i = 1; i < fft_size/2; i++) {
    freq_avg += (std::arg(chunk[i]) / M_PI) * std::abs(chunk[i]);
    #ifndef FASTMODE
      if(std::abs(chunk[i]) > loudest[1]) {
        loudest[0] = i;
        loudest[1] = std::abs(chunk[i]);
      }
//    std::cout << std::abs(chunk[i]) << " ";
//    std::cout << "freq: " << std::arg(chunk[i]) / M_PI << ", amp: " << std::abs(chunk[i]) << std::endl;
    #endif
  }
//  std::cout << "loudest bin: " << loudest[0] << std::endl << std::endl;
//  verbose("calc avg");

  amp_avg = amp_avg / fft_size/2;
  freq_avg = freq_avg / ((fft_size/2) - 1);

  // Calculate variables for spatialisation
  // - Calculate Y value based on spectral information and amplitude
  z = 0.5 * ((((freq_avg * 2.0) - 1.0) * amp_avg) + z);
  #ifdef FASTMODE
    xz[1] = z;
  #else
    value_pair.imag(z);
  #endif
//    verbose("calc z");

  // - Calculate X value based on spectral information, LFO automation,
  //   and number of sources in given range
  // TODO: Should this be here?
  x = (0.8 * ((freq_avg * 2.0) - 1.0)) + (0.2 * x);
  #ifdef FASTMODE
    xz[0] = x;
  #else
    value_pair.real(x);
  #endif
//    verbose("calc x");
}

void Analyzer::fft(CArray& l_chunk) {
  // Cooley–Tukey FFT (in-place, divide-and-conquer)
  // Borrowed from https://rosettacode.org/wiki/Fast_Fourier_transform#C.2B.2B
  const size_t N = l_chunk.size();
  if (N <= 1) return;

  // divide
  CArray even = l_chunk[std::slice(0, N/2, 2)];
  CArray odd = l_chunk[std::slice(1, N/2, 2)];

  // conquer
  fft(even);
  fft(odd);

  // combine
  for (size_t k = 0; k < N/2; ++k)
  {
    Complex t = std::polar(1.0, -2 * M_PI * k / N) * odd[k];
    l_chunk[k] = even[k] + t;
    l_chunk[k+N/2] = even[k] - t;
  }
}

Complex Analyzer::getComplexPair() {
  return value_pair;
}

float *Analyzer::getFloatValues() {
  return xz;
}
