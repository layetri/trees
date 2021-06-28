//
// Created by Daniël Kamp on 23/04/2021.
//

#ifndef CAVES_AUDIOINPUT_H
#define CAVES_AUDIOINPUT_H

#include "Global.h"

#ifdef PLATFORM_TEENSY_40
  //#include <Arduino.h>

  class AudioInput {
    public:
      AudioInput(int pin);
      ~AudioInput();

      int getSample();

    private:
      int adjust(int);
      int scale(int);

      int pin;
      int sample;
  };
#endif
#endif //CAVES_AUDIOINPUT_H
