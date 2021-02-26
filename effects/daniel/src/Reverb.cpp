//
// Created by Daniël Kamp on 25/02/2021.
//

#include "Reverb.h"

Reverb::Reverb(float tail, int samplerate, Buffer *input, Buffer *output) {
  // Assign IO buffer pointers
  this->input = input;
  this->output = output;
  y = new Buffer(samplerate * 10);

  // Calculate delay times, feedback rates and gain ratio
  this->tail = tail;
  calculateRatios();

  // Create various delay lines
  dl1 = new DelayLine(dt1, fb1, samplerate, input);
  dl2 = new DelayLine(dt2, fb2, samplerate, input);
  dl3 = new DelayLine(dt3, fb3, samplerate, input);
  dl4 = new DelayLine(dt4, fb4, samplerate, input);

  // Create a low pass filter
  lpf = new LowPassFilter(600.0, samplerate, y, output);
}

Reverb::~Reverb() {
  delete dl1;
  delete dl2;
  delete dl3;
  delete dl4;
  delete y;
  delete lpf;
}

void Reverb::tick() {
  dl1->tick();
  dl2->tick();
  dl3->tick();
  dl4->tick();
}

void Reverb::process() {
  float smp;
  smp = dl1->process() * n1 + dl2->process() * n2 + dl3->process() * n3 + dl4->process() * n4;
  y->write(smp);
  lpf->process();
}

void Reverb::setTail(float length) {
  tail = length;
  calculateRatios();
}

void Reverb::calculateRatios() {
  n1 = 0.47;
  n2 = 0.32;
  n3 = 0.13;
  n4 = 0.08;

  dl1->setdf((int) (n1 * 10000 * tail), 0.99);
  dl2->setdf((int) (n2 * 10000 * tail), 0.87);
  dl3->setdf((int) (n3 * 10000 * tail), 0.76);
  dl4->setdf((int) (n4 * 10000 * tail), 0.64);
}
