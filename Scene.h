#ifndef SCENE_H
#define SCENE_H

#include <algorithm>
#include <vector>

#include "Light.h"
#include "Shape.h"
#include "Camera.h"
#include "Ray.h"
#include "Intersection.h"

class Scene {
    public:
        std::vector<Shape *> shapes;
        std::vector<Light *> lights;
        Camera camera;

        Scene(std::vector<Shape *>& shapes_,
              std::vector<Light *>& lights_,
              Camera& cam_) : lights(lights_), shapes(shapes_), camera(cam_) { }

        Scene(Camera& cam_) : lights(), shapes(), camera(cam_) { }
            
        void add_light(Light *l) { lights.push_back(l); }
        void add_shape(Shape *s) { shapes.push_back(s); }


        /*
        Checks to see if a ray intersects with objects in our scene. If so,
        an Intersection object is returned containing information about the closest one.
        */
        Intersection findIntersect(const Ray& ray) const;

        ~Scene() { }
};

#endif 