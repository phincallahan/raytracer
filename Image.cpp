#include "Image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STBI_FAILURE_USERMSG

Image::Image(int w, int h) : width(w), height(h) {
    pix = new double[w * h * 3];
}

Image::~Image() { delete pix; }

void Image::setPixel(int i, int j, const vec3& c) {
    if(i >= width || j >= height) {
        throw std::out_of_range("Image out of range");
    }

    double * curr = &pix[(width * j + i) * 3];
    curr[0] = c.x;
    curr[1] = c.y;
    curr[2] = c.z;
}

void Image::addPixel(int i, int j, const vec3& c) {
    if(i >= width || j >= height) {
        throw std::out_of_range("Image out of range");
    }

    double * curr = &pix[(width * j + i) * 3];
    curr[0] += c.x;
    curr[1] += c.y;
    curr[2] += c.z;
}

int Image::writePng(std::string path) const {
    char unsigned * out = new unsigned char[width * height * 3];
    for(int i = 0; i < width * height * 3; i++) {
        // pix[i] = pow(pix[i], 1/2.2);
        out[i] = (unsigned char) fmin(255 * pix[i], 255);
    }

    return stbi_write_png(path.c_str(), width, height, 3, out, 3 * height);
}