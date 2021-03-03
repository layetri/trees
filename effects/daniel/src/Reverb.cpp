//
// Created by Daniël Kamp on 25/02/2021.
//

#include "Reverb.h"
#include <iostream>

Reverb::Reverb(float tail, int samplerate, Buffer *input, Buffer *output) {
  // Assign IO buffer pointers
  this->input = input;
  this->output = output;
  y = new Buffer(input->getSize(), "reverbY");

  // Calculate delay times, feedback rates and gain ratio
  this->samplerate = samplerate;
  this->tail = tail * samplerate / 1000;

  // Create various delay lines
  if(tail >= 2000 && tail < 10000) {
    dl_size = (int) tail / 200;
  } else if (tail < 2000) {
    dl_size = 10;
  } else {
    dl_size = 50;
  }

  for(int i = 0; i < dl_size; i++) {
    dl[i] = new DelayLine((int) (this->tail * pow(0.9, i)), 1.0 - (pow(0.8, i) - 0.05), input);
  }

//  calculateRatios();

  // Create a low pass filter
  lpf = new LowPassFilter(1200.0, samplerate, y, output);
}

Reverb::~Reverb() {
  for(int i = 0; i < dl_size; i++) {
    delete dl[i];
  }

  delete y;
  delete lpf;
}

void Reverb::tick() {
  for(int i = 0; i < dl_size; i++) {
    dl[i]->tick();
  }

  y->tick();
  lpf->tick();
}

void Reverb::process() {
  float smp = 0.0;

  for(int i = 0; i < dl_size; i++) {
    smp += dl[i]->process();
  }
  smp = smp / dl_size;

//  smp = dl[0]->process() * n1 + dl[1]->process() * n2 + dl[2]->process() * n3 + dl[3]->process() * n4;
  y->write(smp);
//  lpf->process();
  output->write(y->getCurrentSample());
}

void Reverb::setTail(float length) {
  // Length in ms, convert to samples
  tail = length * samplerate / 1000.0;
  calculateRatios();
}

void Reverb::calculateRatios() {
  for(int i = 0; i < dl_size; i++) {
    dl[i]->setdf((int) (tail * pow(0.9, i)), 1.0 - (pow(0.8, i) - 0.05));
  }
}
