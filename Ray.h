#ifndef RAY_H
#define RAY_H

#include "vec3.h"
#include <iostream>

class Ray {
    public:
        vec3 origin, dir;
        Ray(vec3 o, vec3 d) : origin(o), dir(normalize(d)) { }
        
        vec3 getPoint(const double t) const { return origin + dir * t; }

        friend std::ostream& operator<<(std::ostream& os, const Ray& r) {
            os << r.origin << " + " << r.dir << "t";
            return os;
        };
};

#endif