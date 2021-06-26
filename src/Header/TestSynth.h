//
// Single-header test synthesizer for audio development purposes.
// Depends on Buffer.h for I/O.
// Created by Daniël Kamp on 25/06/2021.
//

#ifndef CAVES_TESTSYNTH_H
#define CAVES_TESTSYNTH_H

#include <cmath>
#include "Buffer.h"

#include <iostream>

class Synth {
  public:
    Synth(float frequency, int samplerate, Buffer* output) {
      out_buffer = output;
      this->samplerate = samplerate;
      setFrequency(frequency);

      sample = 0;
      phase = 0.0;
      step = 0;
      cnt = 0;
    };
    ~Synth() {};

    // Generate a sample and write to output buffer.
    void tick() {
      cnt++;
//      if(cnt < samplerate / 2) {
        sample = (int16_t) (sin(phase * M_PI * 2.0) * 32768.0);
        out_buffer->write(sample);

        phase += phase_step;
        if (phase > 1.0) {
          phase -= 1.0;
        }
//      }


      if(cnt > samplerate) {
        cnt = 0;
        step++;
        if(step > 3) step = 0;

        setFrequency(mtof(steps[step]));
      }
    };

    void setFrequency(float n_frequency) {
      frequency = n_frequency;
      phase_step = frequency / samplerate;
    };

    float mtof(int note) {
      return pow(2.0, (note - 69.0) / 12.0) * 440.0;
    }

  private:
    Buffer* out_buffer;
    int16_t sample;

    int samplerate;
    float frequency;
    float phase;
    float phase_step;

    int cnt;
    int step;
    int steps[4] {60, 55, 53, 58};
};

#endif //CAVES_TESTSYNTH_H
