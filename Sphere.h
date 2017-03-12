#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
#include "Ray.h"
#include "Material.h"
#include "Intersection.h"

class Sphere : public Shape {
    public:
        vec3 center;
        double radius;
        Material *material;
        Sphere(vec3 pos, double r, Material *mat_) :
            center(pos), radius(r), material(mat_) { }

        //https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
        Intersection intersect(Ray ray) const;
};

#endif