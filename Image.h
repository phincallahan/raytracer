#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <string>

#include "vec3.h"

class Image {
    public:
        Image(int w, int h);
        ~Image();

        void setPixel(int i, int j, const vec3& c);
        int writePng(std::string path) const;

    private:
        int width, height;
        char *pix;
};

#endif