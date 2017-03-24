//https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection

#include "Sphere.h"
#include <cmath>

Intersection Sphere::intersect(Ray ray) const {
    double d = this->distance(ray);
    if(d < 0) {
        return Intersection();
    } else {
        vec3 intersection = ray.getPoint(d);
        vec3 normal = normalize(intersection - this->center);
        return Intersection(d, intersection, normal, this->material);
    }
}

double Sphere::distance(Ray ray) const {
    vec3 diff = ray.origin - center;
    double a = dot(ray.dir, ray.dir);
    double b = 2 * dot(diff, ray.dir);
    double c = dot(diff, diff) - (radius * radius);

    double d = b * b - 4 * a * c;
    if(d < 0) return -1;

    d = sqrt(d);

    double q = 0.5 * (b < 0 ? (-b - d) : (-b + d));
    double r1 = q / a;
    double r2 = c / q;

    if(r1 < 0 && r2 < 0) return -1;

    if (r2 < 0) {
        return r1;
    } else if (r1 < 0 || r2 < r1) {
        return r2;
    } else {
        return r1;
    }
}
