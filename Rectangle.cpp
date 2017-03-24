#include "Rectangle.h"

Intersection Rectangle::intersect(Ray ray) const {
    double d = this->distance(ray);
    if (d < 0) {
        return Intersection();
    } else {
        return Intersection(d, ray.getPoint(d), plane.normal, this->material);
    }
}

double Rectangle::distance(Ray ray) const {
    double t = plane.distance(ray);
    if (t < 0) return t;

    vec3 i = ray.getPoint(t) - plane.point;
    double ul = dot(u, i) / dot(u, u);
    double vl = dot(v, i) / dot(v, v);

    if (ul < 1 && ul > 0 && vl < 1 && vl > 0) {
        return t;
    } else {
        return -1;
    }
}