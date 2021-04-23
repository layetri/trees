#include <iostream>

#include "Header/Pan.h"
#include "Header/LowPassFilter.h"

#include "Header/Buffer.h"
#include "Header/AudioInput.h"

#define samplerate 44100
#define angleDeg 45

//int setup() {
//
//}

int main() {
  AudioInput input1(0);
  AudioInput input2(1);

  Buffer inputL(samplerate * 10, "input");
  Buffer inputR(samplerate * 10, "input");

  Buffer outputL(samplerate * 10, "output");
  Buffer outputR(samplerate * 10, "output");

  LowPassFilter filterL(5000.0, samplerate, &inputL, &outputL);
  LowPassFilter filterR(5000.0, samplerate, &inputR, &outputR);
  filterL.setFrequency(500.0);
  filterR.setFrequency(500.0);

  float panning = -1;
  Pan pan;

  panning = panning + 0.00001;
  if(panning > 1) {
    panning = -1;
  }

  inputL.write(input1.getSample());
  inputR.write(input2.getSample());

  outputL.write(filterL.process() * pan.gainCalL(angleDeg, panning));
  outputR.write(filterR.process() * pan.gainCalR(angleDeg, panning));
}
