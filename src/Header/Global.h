//
// Created by Daniël Kamp on 22/06/2021.
//

#ifndef CAVES_GLOBAL_H
#define CAVES_GLOBAL_H

// Project-specific global code goes here!
#define NUM_INPUTS 6

#include <complex>
#include <valarray>
typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

// =======================================
//  Global.h - Developer Tools
//  © 2021 Daniël Kamp
// =======================================
#define DEVMODE
// Set platform to Darwin x86 (macOS)
#define PLATFORM_DARWIN_X86

// Make sure that there's only one platform at once
#if !defined(PLATFORM_DARWIN_X86)
// Set platform to Arduino
  //#define PLATFORM_ARDUINO_ARM

  // Set platform to Teensy 4.0 (IMXRT1062)
  #define PLATFORM_TEENSY40
#endif

// Global logging function
#ifdef DEVMODE
#if defined(PLATFORM_DARWIN_X86)
    #include <string>
    #include <iostream>
  #else
    #include <Arduino.h>
  #endif

  template <typename T>
  void verbose(const T& message) {
    #if defined(PLATFORM_DARWIN_X86)
      std::cout << message << std::endl;
    #else
      Serial.println(message);
    #endif
  }
#endif

#endif //CAVES_GLOBAL_H
