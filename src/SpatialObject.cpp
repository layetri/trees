//
// Created by Daniël Kamp on 23/06/2021.
//

#include "Header/SpatialObject.h"
#define FFT_SIZE 512

SpatialObject::SpatialObject(Buffer *input, Buffer *left, Buffer *right) {
  input_buffer = input;
  output_left = left;
  output_right = right;

  verbose(input_buffer->getName());
  analyzer = new Analyzer(FFT_SIZE, input);
  verbose(input_buffer);

  gain_left = 0.0;
  gain_right = 0.0;
}

SpatialObject::~SpatialObject() {}

void SpatialObject::processOutputSample() {
  if(input_buffer->getPosition() % FFT_SIZE == 0) {
    // Run the analysis (FFT), get complex location of source
    analyzer->process();
    #ifndef FASTMODE
      location = analyzer->getComplexPair();
    #else
      loc_flt = analyzer->getFloatValues();
    #endif

    // Calculate gain factors based on complex location
    calcGain();
  }

  // Apply gain factors to current sample
  sample_left = ((input_buffer->getCurrentSample() * gain_left) + sample_left) * (1.0 / (NUM_INPUTS * 2));
  sample_right = ((input_buffer->getCurrentSample() * gain_right) + sample_right) * (1.0 / (NUM_INPUTS * 2));
//  sample_right = input_buffer->getCurrentSample() * gain_right;

//  verbose(gain_right);
  // TODO: implement sample processing
  //  - filtering
  //    - use angle/distance
  //  - phase modulation

  // Write processed samples to outputs
  output_left->writeAddition(sample_left);
  output_right->writeAddition(sample_right);
}

void SpatialObject::calcGain() {
  #ifndef FASTMODE
    // Distance -> overall gain
    double gain_mod = 1.0 - (std::abs(location) / 4.0);
    // Real part -> L/R distribution
    double gain = ((location.real() * 0.5) - 0.5) / M_PI;
  #else
    // Use Euclidean distance to calculate abs distance
    double gain_mod = 1.0 - (sqrt(pow(loc_flt[0], 2) + pow(loc_flt[1], 2)) / 4.0);
    // Map L/R distribution from x axis value
    double gain = ((loc_flt[0] * 0.5) - 0.5) / M_PI;
  #endif

  // Do gain multiplication for both channels
  gain_left = (((1.0 - gain) * gain_mod) + gain_left) * 0.5;
  gain_right = ((gain * gain_mod) + gain_right) * 0.5;

  // Branchless gain limiting (instead of slow and untidy conditional chain)
  gain_left = ((gain_left < -1.0) * -1.0) + ((gain_left > 1.0) * 1.0) + ((-1.0 < gain_left && gain_left < 1.0) * gain_left);
  gain_right = ((gain_right < -1.0) * -1.0) + ((gain_right > 1.0) * 1.0) + ((-1.0 < gain_right && gain_right < 1.0) * gain_right);

  std::cout << "freq_dist: " << gain - 0.5 << ", gain_mod: " << gain_mod << ", gain_l: " << gain_left << ", gain_r: " << gain_right << std::endl;
}
