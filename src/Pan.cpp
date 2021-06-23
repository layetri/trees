#include "Header/Pan.h"
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
  delete output_left;
  delete output_right;

  for(auto& item : objects) {
    delete item;
  }
}

void PanningInterface::setAngle(float n_angle_degrees) {
	angle_degrees = n_angle_degrees;

	// Use the angle in degrees to calculate various parameters
	angle = angle_degrees / 360.0f;
	distance = (float) (1 / cos(TWO_PI * angle));
	updateAnglePanning();
}

void PanningInterface::setPanning(float n_panning) {
	panning = n_panning;
	updateAnglePanning();
}

void PanningInterface::updateAnglePanning() {
  angle_panning = panning * tan(TWO_PI * angle);
  a = sqrt(angle_panning * angle_panning + 1);
  b = atan(angle_panning) / TWO_PI;
}

float PanningInterface::gainCalL() {
	gainL = (float) (cos((b + angle) * TWO_PI) * distance / a * 2 / (distance + 1));
	return gainL;
}

float PanningInterface::gainCalR() {
	gainR = (float) (cos((b - angle) * TWO_PI) * distance / a * 2 / (distance + 1));
	return gainR;
}

float *PanningInterface::getSample() {
  float smp[2] = {0.0, 0.0};
  return smp;
}

void PanningInterface::tick() {
  for(auto& channel : objects) {
    channel->processOutputSample();
  }
}
