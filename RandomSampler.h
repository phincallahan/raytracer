#ifndef RANDOM_SAMPLER_H
#define RANDOM_SAMPLER_H

#include "Sampler.h"

class RandomSampler : public Sampler {
    public:
        int xPos, yPos;
        double spanX, spanY;
        RandomSampler(int xs, int xe, int ys, int ye, int ns) : 
            Sampler(xs, xe, ys, ye, ns), xPos(xs-1), yPos(ys),
            spanX((double) xe - xs), spanY((double) ye - ys) { };

        int moreSamples(Sample *s);
};

#endif