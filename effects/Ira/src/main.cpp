#include <iostream>
#include <thread>
#include "jack_module.h"
#include "math.h"
#include "Pan.h"
#include "LowPassFilter.h"
#include "Buffer.h"

#define samplerate 44100
#define angleDeg 45

int main(int argc,char **argv)
{
  // create a JackModule instance
  JackModule jack;

  // init the jack, use program name as JACK client name
  jack.init("example");

  Buffer inputL(samplerate * 10, "input");
  Buffer inputR(samplerate * 10, "input");
  Buffer outputL(samplerate * 10, "output");
  Buffer outputR(samplerate * 10, "output");

  LowPassFilter filterL(5000.0, samplerate, &inputL, &outputL);
  LowPassFilter filterR(5000.0, samplerate, &inputR, &outputR);
  filterL.setFrequency(500.0);
  filterR.setFrequency(500.0);

  float panning = -1;
    Pan mypan;
  
  // int filterdBuf[10000];

  //assign a function to the JackModule::onProces
  jack.onProcess = [&filterL, &filterR, &mypan, &panning, &inputL, &inputR](
    jack_default_audio_sample_t *inBuf,
    // jack_default_audio_sample_t *filterdBuf,
    jack_default_audio_sample_t *outBufL,
    jack_default_audio_sample_t *outBufR,
    jack_nframes_t nframes) {

    for(unsigned int i = 0; i < nframes; i++){
        panning = panning + 0.00001;
      if(panning > 1)
        panning = -1;

    inputL.write(inBuf[i]);
    inputR.write(inBuf[i]);
    // std::cout << filter.process() << std::endl;

    outBufL[i] = filterL.process() * mypan.gainCalL(angleDeg, panning);
    outBufR[i] = filterR.process() * mypan.gainCalR(angleDeg, panning);

  		// std::cout << pan << std::endl;
  		// std::cout << outBufL[i] << ":-:" << outBufR[i] << std::endl;
  	}
    return 0;
  };

  jack.autoConnect();

  //keep the program running and listen for user input, q = quit
  std::cout << "\n\nPress 'q' when you want to quit the program.\n";
  bool running = true;
  while (running)
  {
    switch (std::cin.get())
    {
      case 'q':
        running = false;
        jack.end();
        break;
    }
  }

  //end the program
  return 0;
}
