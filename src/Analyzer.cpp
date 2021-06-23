//
// Created by Daniël Kamp on 22/06/2021.
//

#include "Header/Analyzer.h"

Analyzer::Analyzer(int size, Buffer* input) {
  buffer = input;
  fft_size = size;
  x = 0;
  z = 0;
}

Analyzer::~Analyzer() {

}

void Analyzer::process() {
  double freq_avg, amp_avg;
  amp_avg = 0.0;
  freq_avg = fft_size;

  CArray chunk(fft_size);


  // Do FFT analysis
  for(int i = 0; i < fft_size/2; i++) {
    float val = (buffer->readAhead(i) / 32768.0) - 1.0;

    amp_avg += val;
    chunk[i] = val;
  }
  fft(chunk);


  // Calculate weighed average of spectrum and amplitude
  for(int i = 0; i < fft_size/2; i++) {
    freq_avg += std::arg(chunk[i]) * std::abs(chunk[i]);
  }
  amp_avg = amp_avg / fft_size;
  freq_avg = freq_avg / fft_size;


  // Calculate variables for spatialisation
  // - Calculate Y value based on spectral information and amplitude
  z = 0.5 * ((((freq_avg * 2.0) - 1.0) * amp_avg) + z);
  value_pair.imag(z);

  // - Calculate X value based on spectral information, LFO automation,
  //   and number of sources in given range
  // TODO: Should this be here?
  x = 0.5 * ((x) + x);
  value_pair.real(x);
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
