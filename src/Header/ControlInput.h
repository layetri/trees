//
// Created by Daniël Kamp on 22/06/2021.
//

#ifndef CAVES_CONTROLINPUT_H
#define CAVES_CONTROLINPUT_H

#include "Global.h"

#if !defined(PLATFORM_DARWIN_X86)
  #include <Arduino.h>
#elif defined(PLATFORM_DARWIN_X86)
  #include <cmath>
  #include <random>
  #include <cstdint>
  #include <ctime>
#endif

#include <string>
#include <vector>

#include "Buffer.h"

// Provide a base struct for various exciters
struct Keymap {
    Keymap(char n_target) {
      target = n_target;
    };
    virtual ~Keymap() {};
    virtual bool isOfType(char input) {
      return input == target;
    };
    virtual void execute() {
      // TODO: callback handling stuff
    };

    char target;
    // TODO: add callback stuff
};

// Provide an interface for all the different keymaps
struct ControllerInterface {
    ControllerInterface() = default;
    ~ControllerInterface() {
      for(auto *k : keymaps) {
        delete k;
      }
    }

    void execute(char input) {
      for (auto *k : keymaps) {
        if (k->isOfType(input)) {
          k->execute();
        }
      }
    };
    void import(Keymap* keymap)  {
      keymaps.push_back(keymap);
    }
private:
    std::vector<Keymap*> keymaps;
};

#endif //CAVES_CONTROLINPUT_H
