//
// Created by Daniël Kamp on 23/04/2021.
//

#ifndef CAVES_AUDIOINPUT_H
#define CAVES_AUDIOINPUT_H

//#include <Arduino.h>

class AudioInput {
  public:
    AudioInput(int pin);
    ~AudioInput();

    int getSample();

  private:
    int pin;
    int sample;
};

#endif //CAVES_AUDIOINPUT_H
