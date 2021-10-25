#include "Buffer.h"
#include "Global.h"
#include "Biquad.h"

#include <vector>
#pragma once

class Amplitude {
    public:
        Amplitude(Buffer*, Buffer*);
        ~Amplitude();

        std::vector<int>* DetectOnsets();
        void Hilbert();

    private:
        void initializeBiquads();
        
        Buffer* input;
        Buffer* output;

        Coefficients *c1, *c2, *c3, *c4;
        Biquad *bq1, *bq2, *bq3, *bq4;
        Buffer *link1, *link2, *link3, *link4, *analysis_bus;
};