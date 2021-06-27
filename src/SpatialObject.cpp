//
// Created by Daniël Kamp on 23/06/2021.
//

#include "Header/SpatialObject.h"
#define FFT_SIZE 4096

SpatialObject::SpatialObject(Buffer *input, Buffer *left, Buffer *right) {
  input_buffer = input;
  output_left = left;
  output_right = right;

  analyzer = new Analyzer(FFT_SIZE, input);

  gain_left = 0.0;
  gain_right = 0.0;
  cnt = 0;
}

SpatialObject::~SpatialObject() {}

int16_t* SpatialObject::processOutputSample() {
  // Apply gain factors to current sample
  sample_left = ((input_buffer->getCurrentSample() * gain_left) + sample_left) / (NUM_INPUTS * 2.0);
  sample_right = ((input_buffer->getCurrentSample() * gain_right) + sample_right) / (NUM_INPUTS * 2.0);

  // TODO: implement sample processing
  //  - filtering
  //    - use angle/distance
  //  - phase modulation

  // Write processed samples to outputs
//  output_left->writeAddition(sample_left);
//  output_right->writeAddition(sample_right);

  sample[0] = sample_left;
  sample[1] = sample_right;

  cnt++;

  if(cnt == FFT_SIZE) {
    // Run the analysis (FFT), get complex location of source
    analyzer->process();
    location = analyzer->getFloatValues();

    // Calculate gain factors based on complex location
    calcGain();
    cnt = 0;
  }

  return sample;
}

void SpatialObject::calcGain() {
  // Use Euclidean distance to calculate abs distance
//  double gain_mod = sqrt(pow(location[0] / M_PI, 2) + pow(location[1], 2)) / M_PI;
  double gain_mod = 1.0;
  // Map L/R distribution from x axis value (with bias cause frequency spectrum is weird)
  double gain = (location[0] * M_PI) - 0.2;
  gain = ((gain < 0) * 0.0) + ((gain > 1.0) * 1.0) + ((gain > 0.0 && gain < 1.0) * gain);

  // Do gain multiplication for both channels
  gain_left = (((1.0 - gain) * gain_mod) + gain_left) * 0.5;
  gain_right = ((gain * gain_mod) + gain_right) * 0.5;

  // Branchless gain limiting (instead of slow and untidy conditional chain)
//  gain_left = ((gain_left < -1.0) * -1.0) + ((gain_left > 1.0) * 1.0) + ((-1.0 < gain_left && gain_left < 1.0) * gain_left);
//  gain_right = ((gain_right < -1.0) * -1.0) + ((gain_right > 1.0) * 1.0) + ((-1.0 < gain_right && gain_right < 1.0) * gain_right);

//  std::cout << "freq_dist: " << gain << ", gain_mod: " << gain_mod << ", gain_l: " << gain_left << ", gain_r: " << gain_right << std::endl;
}