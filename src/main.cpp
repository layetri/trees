#include <iostream>

#include "Header/Global.h"

#include "Header/Pan.h"
#include "Header/Analyzer.h"
#include "Header/LowPassFilter.h"

#include "Header/Buffer.h"

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
    jack.init("synth");
    int samplerate = jack.getSamplerate();
    if(samplerate == 0) {
      samplerate = 44100;
    }

    // Create input array for 6 input channels
    Buffer* inputs[6];
    ControllerInterface ci;

    // TODO: make universal Keymap class with "target" and "callback" arguments
    ci.import(new Keymap('e'));
    ci.import(new Keymap('q'));
    ci.import(new Keymap('w'));

    for(int i = 0; i < NUM_INPUTS; i++) {
      inputs[i] = new Buffer(samplerate * 10, &"input-"[i]);
    }

    Buffer outputL(samplerate * 10, "output_L");
    Buffer outputR(samplerate * 10, "output_R");

    // Initialize panning component
    float panning = -1;
    PanningInterface panner(inputs, &outputL, &outputR);

    // Assign the Jack callback function
    jack.onProcess = [&panner, &inputs](
        jack_default_audio_sample_t **inBufArray,
        jack_default_audio_sample_t *outBuf_L,
        jack_default_audio_sample_t *outBuf_R,
        jack_nframes_t nframes
      ) {
        for(unsigned int i = 0; i < nframes; i++) {
          float smp[2] = {0.0, 0.0};

          for(int j = 0; j < NUM_INPUTS; j++) {
            inputs[j]->write(inBufArray[j][i]);
          }

          // TODO: fix Panner.getSample()
          smp[0] = panner.getSample()[0];
          smp[1] = panner.getSample()[1];

          panner.tick();

          outBuf_L[i] = smp[0];
          outBuf_R[i] = smp[1];
        }
        return 0;
    };
    jack.autoConnect();

    verbose("Setup done!");

    bool running = true;
    while(running) {
      panning += 0.00001;
      if (panning > 1) {
        panning = -1;
      }
    }

//    outputL.write(filterL.process() * pan.gainCalL(angleDeg, panning));
//    outputR.write(filterR.process() * pan.gainCalR(angleDeg, panning));
  }
#endif
