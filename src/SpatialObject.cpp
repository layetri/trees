//
// Created by Daniël Kamp on 23/06/2021.
//

#include "Header/SpatialObject.h"

SpatialObject::SpatialObject(Buffer *input, Buffer *left, Buffer *right) {
  input_buffer = input;
  output_left = left;
  output_right = right;

  analyzer = new Analyzer(512, input_buffer);

  gain_left = 0.0;
  gain_right = 0.0;
}

SpatialObject::~SpatialObject() {}

void SpatialObject::processOutputSample() {
  int16_t sample_left, sample_right;

  // Run the analysis (FFT), get complex location of source
  analyzer->process();
  location = analyzer->getComplexPair();

  // Calculate gain factors based on complex location
  calcGain();

  // Apply gain factors to current sample
  sample_left = input_buffer->getCurrentSample() * gain_left;
  sample_right = input_buffer->getCurrentSample() * gain_right;

  // TODO: implement sample processing

  // Write processed samples to outputs
  output_left->write(sample_left);
  output_right->write(sample_right);
}

void SpatialObject::calcGain() {
  // Imaginary part -> overall gain
  double gain_mod = 1.0 - location.imag();
  double gain = location.real();

  a_gain = abs(gain);
  if(gain < 0) {
    gain_left = 0.5 * gain;
    gain_right = 0.5 - gain;
  } else {

  }

  // l_gain = 1 -
  // left = sample * l_gain;
  // right = sample * r_gain;

  // Real part -> panning location
  location.real();

  calcGainLeft();
  calcGainRight();
}

void SpatialObject::calcGainLeft() {

}

void SpatialObject::calcGainRight() {

}
