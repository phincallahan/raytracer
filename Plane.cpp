#include "Plane.h"

Plane::Plane() : normal(vec3(0.0, 0.0, 1.0)), point(vec3(0.0)), material() { }
Plane::Plane(Material *mat) : 
    material(mat), point(vec3(0.0)), normal(vec3(0.0, 0.0, 1.0)) { }
Plane::Plane(vec3 point_, vec3 normal_, Material *mat) : 
    normal(normal_), point(point_), material(mat) { }

Intersection Plane::intersect(Ray ray) const {
    double dt = dot(ray.dir, normal);
    if(dt == 0.0)
        return Intersection();

    double dist = dot((point - ray.origin), normal) / dt;

    if(dist < 0.0)
        return Intersection();
    else {
        return Intersection(dist, ray.getPoint(dist), normal, material);
    }
}

