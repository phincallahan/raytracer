#ifndef SAMPLE_H
#define SAMPLE_H

struct Sample {
    double x, y;
    Sample(double x_, double y_) : x(x_), y(y_) { }
    Sample() : x(), y() { }
};

class Sampler {
    public:
        const int xStart, xEnd, yStart, yEnd, numSamples;
        Sampler(int xstart, int xend, int ystart, int yend, int ns) : 
            xStart(xstart), xEnd(xend), yStart(ystart), yEnd(yend), 
            numSamples(ns) { };
        virtual int moreSamples(Sample * samples) = 0;
};

#endif