//
// Noise Thing v1.0
// © 2021 Daniël Kamp
//

// Include libraries
#include <iostream>
#include <string>
#include "jack_module.h"

#include "Buffer.h"
#include "LowPassFilter.h"
#include "DelayLine.h"
#include "Reverb.h"

// Use std namespace instead of std-littering our code
using namespace std;

// Abstraction to easily provide a text coloring utility
string color(string text, int color) {
  return "\033[" + to_string(color) + "m" + text + "\033[0m";
}

// Main program loop
int main(int argc, char* argv[]) {
  JackModule jack;
  jack.init(argv[0]);
  int samplerate = jack.getSamplerate();
  if(samplerate == 0) {
    samplerate = 44100;
  }

  // Print the welcome screen ✨
  cout << color("System says: ", 32) << "Welcome to localenv. Type " << color("help", 36) << " to get started.\n ";
  cout << color("System says: ", 32) << color("Have a wonderful time! ✨ \n\n", 33);



  Buffer input(samplerate * 10, "input");
  Buffer revBus(samplerate * 10, "revbus");
  Buffer output(samplerate * 10, "output");

  LowPassFilter filter(5000.0, samplerate, &input, &output);
  filter.setFrequency(2000.0);

//  DelayLine dl1(2000, 0.77, &input);
//  DelayLine dl2(689, 0.89, &input);
//  DelayLine dl3(443, 0.94, &input);

  Reverb reverb(2000, samplerate, &revBus, &output);

  float gain = 0.9;
  bool mute = false;

  //assign a function to the JackModule::onProcess
  jack.onProcess = [&filter, &reverb, &revBus, &input, &output, gain, mute](jack_default_audio_sample_t *inBuf,
                           jack_default_audio_sample_t *outBuf, jack_nframes_t nframes) {
      for(int i = 0; i < nframes; i++) {
        float smp, rt;

        if(mute) {
          input.write(0.0);
          revBus.write(0.0);
        } else {
          input.write(inBuf[i]);
          revBus.write(inBuf[i]);
        }

        //smp = filter.process();

        reverb.process();

//        rt = (smp + dl1.process() + dl2.process() + dl3.process()) / 4;

//        outBuf[i] = (filter.process() + output[i]) * gain * 0.5;
//        cout << output.getCurrentSample() << endl;
//        outBuf[i] = revBus[i] * gain;
//        outBuf[i] = smp * gain;
        outBuf[i] = (input.getCurrentSample() * 0.6 + output.getCurrentSample() * 0.4) * gain;

        reverb.tick();
        //filter.tick();

        input.tick();
        revBus.tick();
        output.tick();
//
//        dl1.tick();
//        dl2.tick();
//        dl3.tick();
      }
      return 0;
  };

  jack.autoConnect();

  // Start the TUI loop
  cout << color("> ", 33);
  bool running = true;

  while (running) {
    string command;
    getline(cin, command);

    // Handle scream for help
    if (command == "help") {
      cout <<
           color("play", 35) << "        play noise thing using keyboard commands" << endl <<
           color("exit", 35) << "        exit the program" << endl;
    }
    else if(command == "mute") {
      mute = !mute;
    }
    else if(command == "getf") {
      cout << filter.getFrequency() << endl;
    }
    else if(command == "getd") {
      cout << filter.getDelay() << endl;
    }
    else if(command == "setf") {
      float freq;
      cout << "Type a frequency" << endl << color("> ", 35);

      string intermediate;
      getline(cin, intermediate);
      freq = stof(intermediate);

      filter.setFrequency(freq);
    }
    else if(command == "setg") {
      float gn;
      cout << "Type a gain (0.0 -> 0.99)" << endl << color("> ", 35);

      string intermediate;
      getline(cin, intermediate);
      gn = stof(intermediate);

      gain = gn;
    }
    else if(command == "exit") {
      cout << color("Bye!", 33) << endl;
      break;
    }
    // Handle unknown input
    else {
      cout << "Whoops! " << color(command, 31) << " is not a valid command. Type " << color("help", 36)
             << " for a list of commands." << endl;
    }

    cout << color("> ", 33);
  }
}
