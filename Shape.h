#ifndef SHAPE_H
#define SHAPE_H

#include "Intersection.h"
#include "Ray.h"

class Shape {
    public:
        virtual Intersection intersect(Ray) const = 0;
        virtual double distance(Ray) const = 0;
};

#endif