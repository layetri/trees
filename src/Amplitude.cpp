#include "Header/Amplitude.h"

Amplitude::Amplitude(Buffer* input_buffer, Buffer* output_buffer) {
    input = input_buffer;
    output = output_buffer;

    initializeBiquads();
}

Amplitude::~Amplitude() {
    delete c1, c2, c3, c4, bq1, bq2, bq3, bq4, link1, link2, link3, link4;
}

void Amplitude::initializeBiquads() {
    // TODO: implement linking buffers (cri)
    int len = input->getSize();
    link1 = new Buffer(len);
    // link2 = new Buffer(len);
    // link3 = new Buffer(len);
    // link4 = new Buffer(len);
    analysis_bus = new Buffer(len);

    // Left channel
    c1 = new Coefficients(1.94632, -0.94657, 0.94657, -1.94632, 1.0);
    c2 = new Coefficients(0.83774, -0.06338, 0.06338, -0.83774, 1.0);
    bq1 = new Biquad(1, c1, input, link1);
    bq2 = new Biquad(1, c1, link1, analysis_bus);
    
    // Right channel
    // c1 = new Coefficients(-0.02569, 0.260502, -0.260502, 0.02569, 1.0);
    // c2 = new Coefficients(1.8685, -0.870686, 0.870686, -1.8685, 1.0);
    // bq1 = new Biquad(1, c1, input, link3);
    // bq2 = new Biquad(1, c1, link3, link4);
}

std::vector<int>* Amplitude::DetectOnsets() {
    std::vector<int> onsets;

    for(int i = 0; i < input->getSize(); i++) {
        input->getSample(i);
        Hilbert();
        analysis_bus->getSample(i);
    }

    return &onsets;
}

void Amplitude::Hilbert() {
    bq1->process();
    bq2->process();
}