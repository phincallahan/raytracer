#ifndef LIGHT_H
#define LIGHT_H

#include "vec3.h"

class Light {
    public:
        vec3 color, pos;
        Light(vec3 p, vec3 col) : pos(p), color(col) { }
};

#endif