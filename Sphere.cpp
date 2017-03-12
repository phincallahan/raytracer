//https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection

#include "Sphere.h"
#include <cmath>

Intersection Sphere::intersect(Ray ray) const {
    vec3 diff = ray.origin - center;
    double a = dot(ray.dir, ray.dir);
    double b = 2 * dot(diff, ray.dir);
    double c = dot(diff, diff) - (radius * radius);

    double d = b * b - 4 * a * c;
    if(d < 0) {
        return Intersection();
    }

    d = sqrt(d);

    double q = 0.5 * (b < 0 ? (-b - d) : (-b + d));
    double r1 = q / a;
    double r2 = c / q;

    if(r1 < 0 && r2 < 0) return Intersection();

    double distance;
    if (r2 < 0) {
        distance = r1;
    } else if (r1 < 0 || r2 < r1) {
        distance = r2;
    } else {
        distance = r1;
    }

    vec3 intersection = ray.origin + ray.dir * distance;
    vec3 normal = intersection - this->center;
    normal.normalize();

    return Intersection(distance, intersection, normal, this->material);
}
