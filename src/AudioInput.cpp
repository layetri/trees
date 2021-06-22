//
// Created by Daniël Kamp on 23/04/2021.
//


#include "Header/AudioInput.h"

#ifdef PLATFORM_TEENSY_40
  AudioInput::AudioInput(int pin) {
    this->pin = pin;
    sample = 0;
    //pinMode(pin, INPUT);
  }

  AudioInput::~AudioInput() {

  }

  int AudioInput::getSample() {
    int smp = analogRead(pin);

    smp = adjust(smp);
    smp = scale(smp);

    return sample;
  }

  // Auto-adjust the incoming signal
  int AudioInput::adjust(int value) {
    int val = value;
    return val;
  }

  // Scale the incoming signal to a 16-bit integer for internal processing
  int AudioInput::scale(int value) {
    int val = value;
    return val;
  }
#endif