#include "Header/PanningInterface.h"
#define TWO_PI (2 * M_PI)

PanningInterface::PanningInterface(Buffer** buffers, Buffer* n_output_left, Buffer* n_output_right) {
  output_left = n_output_left;
  output_right = n_output_right;

  // Initialize desired number of input channels
  for(int i = 0; i < NUM_INPUTS; i++) {
    objects[i] = new SpatialObject(buffers[i], output_left, output_right);
  }
}

PanningInterface::~PanningInterface() {
  for(auto& item : objects) {
    delete item;
  }
}

void PanningInterface::process() {
  for(auto& channel : objects) {
    channel->processOutputSample();
  }
}

void PanningInterface::tick() {
  // TODO: Implement Flush-and-Add
  output_left->tick();
  output_right->tick();

  output_left->flush();
  output_right->flush();
}
