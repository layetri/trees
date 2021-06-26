#include <iostream>

#include "Header/Global.h"
#include "Header/Buffer.h"

#include "Header/LowPassFilter.h"
#include "Header/PanningInterface.h"
#include "Header/Reverb.h"

#include "Header/TestSynth.h"

#if defined(PLATFORM_TEENSY_40)
  #include "Header/AudioInput.h"
  #include <Arduino.h>

  #define samplerate 44100
  #define angleDeg 45

  //int setup() {
  //
  //}
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

  int main() {
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
#elif defined(PLATFORM_DARWIN_X86)
  #include "Header/ControlInput.h"
  #include "Header/jack_module.h"

  #define angleDeg 45

  int main() {
    // Do Jack setup
    JackModule jack;
    jack.init("caves");
    int samplerate = jack.getSamplerate();
    if(samplerate == 0) {
      samplerate = 44100;
    }

    // Create input array for 6 input channels
    Buffer* inputs[6];
    for(int i = 0; i < NUM_INPUTS; i++) {
      inputs[i] = new Buffer(samplerate, &"input-"[i]);
    }

    // Initialize output busses and FX
    Buffer revBusL(samplerate * 10, "revbus_L");
    Buffer revBusR(samplerate * 10, "revbus_R");
    Buffer outputL(samplerate, "output_L");
    Buffer outputR(samplerate, "output_R");

    Reverb reverb_L(4000., samplerate, &revBusL, &outputL);
    Reverb reverb_R(4000., samplerate, &revBusR, &outputR);


    // For testing purposes, initialize internal synth
    Synth voice1(220.0, samplerate, inputs[0]);


    // Initialize panning component
    float panning = -1;
    PanningInterface panner(inputs, &revBusL, &revBusR);

    ControllerInterface ci;


    // TODO: make universal Keymap class with "target" and "callback" arguments
    ci.import(new Keymap('e'));
    ci.import(new Keymap('q'));
    ci.import(new Keymap('w'));

    bool bypass = false;

    // Assign the Jack callback function
    jack.onProcess = [&panner, &inputs, &revBusL, &revBusR, &outputL, &outputR, &reverb_L, &reverb_R, &bypass, &voice1](
        jack_default_audio_sample_t **inBufArray,
        jack_default_audio_sample_t *outBuf_L,
        jack_default_audio_sample_t *outBuf_R,
        jack_nframes_t nframes
      ) {
        for(unsigned int i = 0; i < nframes; i++) {
          for(int j = 0; j < NUM_INPUTS; j++) {
            inputs[j]->tick();
            #ifndef DEVMODE
              inputs[j]->write(inBufArray[j][i] * 32768.0);
            #endif
          }

          #ifdef DEVMODE
            voice1.tick();
          #endif

          // Run the panning algorithm
          panner.process();
          reverb_L.process();
          reverb_R.process();

          // Get the processed samples and write to output
          if(!bypass) {
            outBuf_L[i] = (((revBusL.getCurrentSample() + revBusL.readBack(1)) * 0.25) + (outputL.getCurrentSample() * 0.5)) / 32768.0;
            outBuf_R[i] = (((revBusR.getCurrentSample() + revBusR.readBack(1)) * 0.25) + (outputR.getCurrentSample() * 0.5)) / 32768.0;
          } else {
//            outBuf_L[i] = revBusL.getCurrentSample() / 32768.0;
            outBuf_L[i] = inputs[0]->getCurrentSample() / 32768.0;
//            outBuf_R[i] = revBusR.getCurrentSample() / 32768.0;
            outBuf_R[i] = inputs[0]->getCurrentSample() / 32768.0;
          }

          panner.tick();
          reverb_L.tick();
          reverb_R.tick();
        }
        return 0;
    };
    jack.autoConnect();

    bool running = true;
    while(running) {
      char cmd;
      std::cout << "Bypass: ";
      std::cin >> cmd;

      bypass = cmd == 't';

      std::cout << "bypass is now " << bypass << std::endl;
    }

//    outputL.write(filterL.process() * pan.gainCalL(angleDeg, panning));
//    outputR.write(filterR.process() * pan.gainCalR(angleDeg, panning));
  }
#endif
