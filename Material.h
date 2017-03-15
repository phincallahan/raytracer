#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"
/*
A class describing material properties of an object. Possible properties include color,
transparent vs opaque, reflectivity, and specular and diffuse lighting amounts.
kt is the refraction index of a material, kr is the reflectivity, and kd and ks
are the diffuse and specular lighting ratios, respectively. If kt > 0, the object is
transparent, otherwise it is opaque.
*/
class Material
{
    public:
        double kt, kr, ks, kd;
        explicit Material(double kt_, double kr_, double ks_, double kd_) :
            kt(kt_), kr(kr_), ks(ks_), kd(kd_) { }

        virtual vec3 getColor() const = 0;
};

class ColorMaterial : public Material
{
    public:
        vec3 color;
        vec3 getColor() const { return this->color; }

        ColorMaterial(vec3 color_, double kt_, double kr_, double ks_, double kd_) :
            color(color_), Material(kt_, kr_, ks_, kd_) { }
};

#endif
