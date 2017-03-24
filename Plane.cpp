#include "Plane.h"

Plane::Plane() : normal(vec3(0.0, 0.0, 1.0)), point(vec3(0.0)), material() { }
Plane::Plane(Material *mat) : 
    material(mat), point(vec3(0.0)), normal(vec3(0.0, 0.0, 1.0)) { }
Plane::Plane(vec3 p, vec3 n, Material *mat) : 
    normal(normalize(n)), point(p), material(mat) { }
Plane::Plane(vec3 p, vec3 n) : normal(normalize(n)), point(p), material() { }

Intersection Plane::intersect(Ray ray) const {
    double d = this->distance(ray);
    if(d < 0.0)
        return Intersection();
    else {
        double side = dot(ray.dir, normal);
        side = side < 0 ? 1 : -1;
        return Intersection(d, ray.getPoint(d), side * normal, material);
    }
}

double Plane::distance(Ray ray) const {
    double dt = dot(ray.dir, normal);
    if (dt == 0.0) {
        return -1;
    } else {
        return dot((point - ray.origin), normal) / dt;
    }
}

