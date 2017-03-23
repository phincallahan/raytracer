#include <cmath>
#include "Camera.h"

Camera::Camera(double fovy_) : scale(tan(fovy_)) { }

//Ported for Josh's camera code
void Camera::lookAt(vec3 target, double rho, double phi, double theta) {
    vec3 yStd(0.0, 1.0, 0.0), zStd(0.0, 0.0, 1.0);
    vec3 z = vec3::Spherical(1.0, phi, theta);
    vec3 y = vec3::Spherical(1.0, M_PI / 2.0 - phi, theta + M_PI);            

    this->rot = Matrix33::BasisRotation(yStd, zStd, y, z);
    this->pos = z * rho + target;
}

Ray Camera::getRay(Sample s) const {
    double x = (2 * s.x - 1) * scale; 
    double y = (1 - 2 * s.y) * scale; 

    vec3 dir = normalize(rot * vec3(x, y, -1));

    return Ray(pos, dir);
}