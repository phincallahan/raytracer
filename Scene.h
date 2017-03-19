#ifndef SCENE_H
#define SCENE_H

#include <algorithm>

#include "Light.h"
#include "Shape.h"
#include "Camera.h"

using std::vector;

class Scene {
    public:
        vector<Shape *> shapes;
        vector<Light *> lights;
        Camera camera;

        Scene(vector<Shape *>& shapes_,
              vector<Light *>& lights_,
              Camera& cam_) : lights(lights_), shapes(shapes_), camera(cam_) { }

        Scene(Camera& cam_) : lights(), shapes(), camera(cam_) { }
            
        void add_light(Light *l) { lights.push_back(l); }
        void add_shape(Shape *s) { shapes.push_back(s); }

        ~Scene() { }
};

#endif 