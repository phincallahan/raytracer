#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Plane.h"

class Rectangle : public Plane {
    public: 
        vec3 v, u;
        Plane plane;

        Material * material;

        Rectangle(vec3 p, vec3 v_, vec3 u_, Material *m) : 
            plane(p, cross(v_, u_)), v(v_), u(u_), material(m) { }

        Intersection intersect(Ray ray) const;
        double distance(Ray ray) const;
};

#endif 