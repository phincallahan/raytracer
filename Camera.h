#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include "vec3.h"
#include "Matrix33.h"

class Camera {
    public: 
        int width, height;
        double scale;
        vec3 pos;
        Matrix33 rot;

        Camera(double fovy, int width, int height);

        //Ported for Josh's camera code
        void lookAt(vec3 target, double rho, double phi, double theta);
        Ray getRay(double screen_x, double screen_y) const;
};

#endif 