//
// Created by Daniël Kamp on 23/04/2021.
//


#include "Header/AudioInput.h"

AudioInput::AudioInput(int pin) {
  this->pin = pin;
  sample = 0;
  //pinMode(pin, INPUT);
}

AudioInput::~AudioInput() {

}

int AudioInput::getSample() {
//  sample = analogRead(pin);
  return sample;
}