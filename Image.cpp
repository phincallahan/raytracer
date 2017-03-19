#include "Image.h"
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STBI_FAILURE_USERMSG

Image::Image(int w, int h) : width(w), height(h) {
    pix = new char[w * h * 3];
}

Image::~Image() { delete pix; }

void Image::setPixel(int i, int j, const vec3& c) {
    pix[(width * i + j) * 3] =     fmin((int)(255 * c.x), 255);
    pix[(width * i + j) * 3 + 1] = fmin((int)(255 * c.y), 255);
    pix[(width * i + j) * 3 + 2] = fmin((int)(255 * c.z), 255);
}

int Image::writePng(std::string path) const {
    return stbi_write_png(path.c_str(), width, height, 3, pix, 3 * height);
}