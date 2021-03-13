#include <iostream>
#include <thread>
#include "jack_module.h"
#include "math.h"
#include "Pan.h"
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

  float panning = -1;
  	//   float pan = -0.6; //-1 = L, 1 = R
	  // float angle = 28; //Graden
    Pan mypan(angleDeg, panning);
  //assign a function to the JackModule::onProces
  jack.onProcess = [&mypan, &panning](jack_default_audio_sample_t *inBuf,
    jack_default_audio_sample_t *outBufL,jack_default_audio_sample_t *outBufR, jack_nframes_t nframes) {

    for(unsigned int i = 0; i < nframes; i++){
        panning = panning + 0.00001;
      if(panning > 1)
        panning = -1;

     // outBufL[i] = inBuf[i] * gainL;
     // outBufR[i] = inBuf[i] * gainR;
		// float angle = angleDeg / 360.;
		// float anglePan = pan * (tan(PI_2 * angle));
		// float afstand = (1 / cos(PI_2 * angle));
		
		// float a = sqrt(anglePan * anglePan + 1);
		// float b = atan(anglePan)/PI_2;
  		
		// float gainL = (cos((b+angle)*PI_2)*afstand/a*2/(afstand+1));
		// // float gainR = (cos((b-angle)*PI_2)*afstand/a*2/(afstand+1));

		// if(gainL < 0)
		// 	gainL = 0;
		// if(gainR < 0)
		// 	gainR = 0; 

    outBufL[i] = inBuf[i] * mypan.gainCalL(angleDeg, panning);
    outBufR[i] = inBuf[i] * mypan.gainCalR(angleDeg, panning);
  		
  		// std::cout << pan << std::endl;
  		// std::cout << outBufL[i] << ":-:" << outBufR[i] << std::endl;
  	}
    return 0;
  };

  // mypan.setangleDeg(angleDeg);
  // mypan.setpanning(panning);

  // mypan.getangleDeg();
  // mypan.getangleDeg();

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
      // zzzcase 'p':

      //   std::cout << "welkom" << std::endl;
      //   // std::cin << angleDeg;
      //   // std::cin << panning;

      //   mypan.setangleDeg(angleDeg);
      //   mypan.setpanning(panning);
        
      //   mypan.getangleDeg();
      //   mypan.getangleDeg();
    }
  }

  //end the program
  return 0;
} // main()
