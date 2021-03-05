#include <iostream>
#include <thread>
#include "jack_module.h"
#include "math.h"
/*
 * NOTE: jack2 needs to be installed
 * jackd invokes the JACK audio server daemon
 * https://github.com/jackaudio/jackaudio.github.com/wiki/jackd(1)
 * on mac, you can start the jack audio server daemon in the terminal:
 * jackd -d coreaudio
 */
// 10 seconds if samplerate = 44100

#define angleDeg 45

int main(int argc,char **argv)
{
  // create a JackModule instance
  JackModule jack;

  // init the jack, use program name as JACK client name
  jack.init("example");

  float delayBuffer1[DELAY_SIZE];
  float delayBuffer2[DELAY_SIZE];
  int writeHead = 0;
  int readHead = 0;
  float pan = -1;
  	//   float pan = -0.6; //-1 = L, 1 = R
	  // float angle = 28; //Graden

  //assign a function to the JackModule::onProces
  jack.onProcess = [&delayBuffer1, &delayBuffer2, &writeHead, &readHead, &pan](jack_default_audio_sample_t *inBuf,
    jack_default_audio_sample_t *outBufL,jack_default_audio_sample_t *outBufR, jack_nframes_t nframes) {

  	for(unsigned int i = 0; i < nframes; i++){
  		pan = pan + 0.00001;
  		if(pan > 1)
  			pan = -1;

  		float angle = angleDeg / 360.;
		float anglePan = pan * (tan(PI_2 * angle));
		float afstand = (1 / cos(PI_2 * angle));
		
		float a = sqrt(anglePan * anglePan + 1);
		float b = atan(anglePan)/PI_2;
  		
		float gainL = (cos((b+angle)*PI_2)*afstand/a*2/(afstand+1));
		float gainR = (cos((b-angle)*PI_2)*afstand/a*2/(afstand+1));

		if(gainL < 0)
			gainL = 0;
		if(gainR < 0)
			gainR = 0; 

  		outBufL[i] = inBuf[i] * gainL;
  		outBufR[i] = inBuf[i] * gainR;
  		
  		std::cout << pan << std::endl;
  		std::cout << gainL << ":-:" << gainR << std::endl;
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
} // main()
