#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Material.h"
#include "vec3.h"

class Intersection {
    public:
        vec3 normal, pos;
        double distance;
        Material * material;
        Intersection() :
            distance(-1), normal(vec3()), pos(vec3()), material() { }
        Intersection(double d, vec3 pos_, vec3 normal_, Material * mat_) :
            distance(d), normal(normal_), pos(pos_), material(mat_) { }
};

#endif