#include "Material.h"
#include <cmath>


double TransparentMaterial::fresnel(vec3 incoming, vec3 normal, bool inside) {
    double c1 = -dot(normal, incoming);
    double n1 = 1.0;
    double n2 = kt;
    if (inside) {
        n1 = kt;
        n2 = 1.0;
    }
    double c2 = sqrt(1.0 - pow(n1 / n2, 2.0) * (1.0 - pow(c1, 2.0)));

    // In the case of total internal reflection
    double k = 1.0 - pow(n1 / n2, 2.0) * (1.0 - pow(c1, 2.0));
    if (k < 0 ) {
        return 1.0;
    }
    // Setup fresnel equations and average them to get the ratio of reflected light
    double fres1 = pow((n2 * c1 - n1 * c2) / (n2 * c1 + n1 * c2), 2.0);
    double fres2 = pow((n1 * c2 - n2 * c1) / (n1 * c2 + n2 * c1), 2.0);
    return 0.5 * (fres1 + fres2);
}

vec3 TransparentMaterial::refract(vec3 incoming, vec3 normal, bool inside) {
    double c1 = -dot(normal, incoming);
    double currentMedium = 1.0;
    double newMedium;
    if (inside) {
        currentMedium = kt;
        newMedium = 1.0;
    }
    double n = currentMedium / kt;

    //Total internal reflection
    if (1.0 - pow(n, 2.0) * (1.0 - pow(c1, 2.0)) < 0)
        return vec3(0.0);

    double c2 = sqrt(1.0 - pow(n, 2.0) * (1.0 - pow(c1, 2.0)));
    return (n * incoming) + (n * c1 - c2) * normal;
}

vec3 TransparentMaterial::getColor() const { return vec3(1.0); }
vec3 TransparentMaterial::getColor(Ray ray, Ray surfaceNormal, traceFunc trace) const {
    vec3 normal = surfaceNormal.dir;
    vec3 pos = surfaceNormal.pos;

    // Check if within or outside of sphere
    bool inside = false;
    if (dot(ray.dir, intersection.normal) > 0) {
        normal *= -1;
        inside = true;
    }

    // Calculate ratio of reflected light
    double fresAmount = fresnel(ray.dir, normal, inside);

    // Find transmission ray direction
    vec3 refractDir = refract(ray.dir, normal, inside);
    refractDir.normalize();

    if (refractDir != vec3(0.0)) {
        Ray refractRay (intersection.pos + refractDir *.001, refractDir);
        refractColor = trace(refractRay, cameraPos, shapes, lights, depth + 1);

        // Determine color at point of intersection
        vec3 localColor = localLighting(intersection, cameraPos, shapes, lights);
        vec3 color =  localColor + reflColor * fresAmount +
                        refractColor * (1.0 - fresAmount);
        return color;
    } else {
        // Total internal reflection, return only the reflected color and local lighting
        vec3 color =  reflColor * intersection.material->kr;
        return color;
    }
}

vec3 DiffuseMaterial::getColor(Ray ray, Ray surfaceNormal, traceFunc trace) const {
    
}