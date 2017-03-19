#include "Scene.h"

Intersection Scene::findIntersect(const Ray &ray) const {
    Intersection i, closest;
    for( auto& shape : shapes) {
        i = shape->intersect(ray);
        if(i.distance > 0 &&
            (closest.distance < 0 || closest.distance > i.distance)) {
                closest = i;
        }
    }

    return closest;
}
