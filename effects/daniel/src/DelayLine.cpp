//
// Created by Daniël Kamp on 25/02/2021.
//

#include "DelayLine.h"

DelayLine::DelayLine(int delayTime, float feedback, int samplerate, Buffer *input) {
  this->feedback = feedback;
  position = 0;

  x = input;
  y = new Buffer(samplerate * 10);
}

DelayLine::~DelayLine() {
  delete y;
}

// Increment the buffer position
void DelayLine::tick() {
  if(position < x->getSize()) {
    position++;
  } else {
    position -= x->getSize();
  }
}

float DelayLine::process() {
  float sample;
  // Run the delay line
  sample = x->getSample(position - delayTime) + (y->getSample(position - delayTime) * feedback);

  // Store the sample in the output buffer
  y->write(sample);
  y->tick();

  // Return the sample
  return sample;
}

void DelayLine::setDelayTime(int delayTime) {
  this->delayTime = delayTime;
}

void DelayLine::setdf(int delayTime, float feedback) {
  this->delayTime = delayTime;
  this->feedback = feedback;
}
