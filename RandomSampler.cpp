#include "RandomSampler.h"
#include <cstdlib>

int RandomSampler::moreSamples(Sample* samples) {
    if(++xPos == xEnd) {
        xPos = xStart;
        yPos++;
    } 

    if (yPos == yEnd) return 0;

    for(int i = 0; i < numSamples; i++) {
        samples[i].x = xPos + ((float) rand() / RAND_MAX) - .5; 
        samples[i].y = yPos + ((float) rand() / RAND_MAX) - .5; 
    }

    return 1;
}