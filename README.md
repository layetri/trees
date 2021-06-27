# Caves
Caves is an intuitive tool for spatialization. It allows musicians to put mixing in the hands of a smart algorithm, so they can focus on what really matters: making music.

Caves uses a number of analysis techniques to place incoming sounds somewhere in the virtual listening space. In the future, presets may be added to allow for various ways of distributing the sources in space (band formation, orchestral formation, etc).

Caves is designed to work both as a standalone application as well as a Eurorack module. The implementation for Eurorack is still a work in progress however, and will be published along with schematics and a B.O.M. at a later point in time.

### How to use
You literally put sound into it. That's all. No parameters, no knobs, no setup. Caves works plug and play with all your sounds.

<small>You do, however, need the JACK Audio Connection Kit, which can be found [here](https://jackaudio.org).</small><br>
<small>Should you want to use a different number of input ports, this can be achieved by changing the `NUM_INPUTS` macro in `/src/Header/global.h` and re-compiling.

### Credits
- Caves uses an FFT algorithm taken from [here](https://rosettacode.org/wiki/Fast_Fourier_transform#C.2B.2B).
- The concept and part of the code was (co-)developed by Ira Helfferich