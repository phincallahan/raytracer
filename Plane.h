#ifndef PLANE_H
#define PLANE_H

#include "Shape.h"
#include "vec3.h"
#include "Material.h"

class Plane : public Shape {
    public: 
        vec3 point, normal;
        Material *material;
        Plane();
        Plane(Material *mat);
        Plane(vec3 point, vec3 normal, Material *mat);
        Intersection intersect(Ray ray) const;
};

#endif