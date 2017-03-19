#include <iostream>
#include <cmath>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STBI_FAILURE_USERMSG

#include "vec3.h"
#include "Matrix33.h"
#include "Ray.h"
#include "Material.h"
#include "Camera.h"
#include "Sphere.h"
#include "Shape.h"
#include "Intersection.h"
#include "Light.h"
#include "Plane.h"
#include "Scene.h"

using namespace std;

/* Returns the reflected vector for an incoming vector at a point of intersection */
vec3 reflectAbout(vec3 incoming, vec3 axis) {
    return 2 * dot(axis, incoming) * axis - incoming;
}

/* Determines the ray direction for a transmission ray in the case of refraction */
vec3 refractAt(vec3 incoming, Intersection intersection, bool inside) {
    double c1 = -dot(intersection.normal, incoming);
    double currentMedium = 1.0;
    double newMedium;
    if (inside) {
        currentMedium = intersection.material->kt;
        newMedium = 1.0;
    }
    double n = currentMedium / intersection.material->kt;

    //Total internal reflection
    if (1.0 - pow(n, 2.0) * (1.0 - pow(c1, 2.0)) < 0)
        return vec3(0.0);

    double c2 = sqrt(1.0 - pow(n, 2.0) * (1.0 - pow(c1, 2.0)));
    return (n * incoming) + (n * c1 - c2) * intersection.normal;
}

/* Uses the Phong Reflection Model */
vec3 localLighting(Intersection intersect, Scene scene) {
    vec3 camDir = scene.camera.pos - intersect.pos, color;
    camDir.normalize();

    for( auto& light: scene.lights) {
        vec3 lightDir = light->pos - intersect.pos;
        lightDir.normalize();

        if(dot(intersect.normal, lightDir) < 0)
            continue;

        Ray shadowRay (intersect.pos + lightDir * .0001, lightDir);
        Intersection shadowIntersection = scene.findIntersect(shadowRay);

        if (shadowIntersection.distance > 0)
            continue;

        vec3 reflected = reflectAbout(lightDir, intersect.normal);
        reflected.normalize();

        // Diffuse
        color += intersect.material->getColor() *
                 intersect.material->kd *
                 light->color *
                 dot(lightDir, intersect.normal);

        // Specular
        color += light->color *
                 intersect.material->ks *
                 pow(dot(camDir, reflected), 64.0);
    }

    return vec3(
        color.x > .1 ? color.x : .1,
        color.y > .1 ? color.y : .1,
        color.z > .1 ? color.z : .1
    );
}

/*
Determines the ratios of reflected and refracted light seen at a point of intersection
for a transparent object. This function returns the ratio of reflective light.
The refractive light amount will be 1 - this amount.
*/
double fresnel(vec3 incoming, Intersection intersection, bool inside) {
    double c1 = -dot(intersection.normal, incoming);
    double n1 = 1.0;
    double n2 = intersection.material->kt;
    if (inside) {
        n1 = intersection.material->kt;
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

#define MAX_RAY_DEPTH 8

/*
Recursively casts rays in a provided direction. Each trace returns colors
which will be combined to light pixels on the screen.
*/
vec3 trace(const Ray &ray, Scene& scene, int depth) {
    if(depth >= MAX_RAY_DEPTH) return vec3(0.0);

    Intersection intersection = scene.findIntersect(ray);
    vec3 cameraPos = scene.camera.pos;

    if(intersection.distance < 0)
        return vec3(0.0);

    // Get the reflected ray and calculate color
    vec3 reflColor = vec3(0.0);
    if (intersection.material->kr > 0.0) {
        vec3 reflDir = reflectAbout(-ray.dir, intersection.normal);
        reflDir.normalize();

        Ray reflRay (intersection.pos + reflDir * .001, reflDir);
        reflColor = trace(reflRay, scene, depth + 1);
    }

    // Get the refracted rays and calculate color
    vec3 refractColor = vec3(0.0);
    double fresAmount = 0.0;
    if (intersection.material->kt > 0) {
        // Check if within or outside of sphere
        bool inside = false;
        if (dot(ray.dir, intersection.normal) > 0) {
            intersection.normal = -intersection.normal;
            inside = true;
        }
        // Calculate ratio of reflected light
        fresAmount = fresnel(ray.dir, intersection, inside);
        // Find transmission ray direction
        vec3 refractDir = refractAt(ray.dir, intersection, inside);
        refractDir.normalize();

        if (refractDir != vec3(0.0)) {
            Ray refractRay (intersection.pos + refractDir *.001, refractDir);
            refractColor = trace(refractRay, scene, depth + 1);

            // Determine color at point of intersection
            vec3 localColor = localLighting(intersection, scene);
            vec3 color =  localColor + reflColor * fresAmount +
                            refractColor * (1.0 - fresAmount);
            return color;
        } else {
            // Total internal reflection, return only the reflected color and local lighting
            vec3 localColor = localLighting(intersection, scene);
            vec3 color =  localColor + reflColor * intersection.material->kr;
            return color;
        }
    }

    // No refraction. Find the local color (specular, diffuse, ambient)
    vec3 localColor = localLighting(intersection, scene);
    vec3 color =  localColor + reflColor * intersection.material->kr;

    return color;
}

int main() {
    int WIDTH = 1024, HEIGHT = 1024;

    unsigned char * png;
    png = (unsigned char*) malloc(WIDTH * HEIGHT * 3.0);

    // Setup a scene
    ColorMaterial material1(vec3(1.0, 0.3, 0.3), 0.0, 0.8, 1.0, 1.0);
    ColorMaterial material2(vec3(0.0, 1.0, 0.0), 1.01, 0.8, 1.0, 1.0);
    ColorMaterial material3(vec3(1.0, 0.0, 0.0), 0.0, 0.3, 1.0, 1.0);
    ColorMaterial material4(vec3(0.8, 0.2, 1.0), 0.0, 0.8, 1.0, 1.0);
    ColorMaterial material5(vec3(1.0, 1.0, 1.0), 0.0, 0.0, 1.0, .6);

    Sphere sphere1(vec3(0.0, 0.0, 0.0), 0.45, &material1);
    Sphere sphere2(vec3(-4.0, -0.5, -0.65), .35, &material2);
    Sphere sphere3(vec3(-1.0, -0.5, -0.75), .25, &material3);
    Sphere sphere4(vec3(.75, 2.0, -.4), 0.6, &material4);
    Plane plane(vec3(0.0, 0.0, -1.0), vec3(0.0, 0.0, 1.0), &material5);

    vector<Shape *> shapes(5);
    shapes[0] = &sphere1;
    shapes[1] = &sphere2;
    shapes[2] = &sphere3;
    shapes[3] = &sphere4;
    shapes[4] = &plane;

    Light light1(vec3(0.0, 6.0, 2.0), vec3(0.7));
    Light light2(vec3(-8.0, -6.0, 2.0), vec3(0.5));

    vector<Light *> lights(2);
    lights[0] = &light1;
    lights[1] = &light2;

    vec3 target = vec3(0.0, 0.0, 0.0);

    Camera cam(M_PI/12, WIDTH, HEIGHT);
    cam.lookAt(target, 10.0, M_PI/2.0, M_PI);

    Scene scene(shapes, lights, cam);

    //Generate the initial rays. One for each pixel in the screen.
    for(int i = 0; i < WIDTH; i++) {
        for(int j = 0; j < HEIGHT; j++) {

            vec3 c (0.0);
            double GRID_SIZE = 3;
            for(int k = 0; k < GRID_SIZE; k++) {
                for(int l = 0; l < GRID_SIZE; l++) {
                    double y_off = (0.5/GRID_SIZE) + (l/GRID_SIZE);
                    double x_off = (0.5/GRID_SIZE) + (k/GRID_SIZE);

                    Ray ray = cam.getRay(j + x_off, i + y_off);

                    vec3 col = trace(ray, scene, 0);
                    c += col;
                }
            }

            c = c / (double)(GRID_SIZE * GRID_SIZE);

            double colors[3] = { c.x, c.y, c.z };
            png[(WIDTH * i + j) * 3] = fmin((int)(255 * c.x), 255);
            png[(WIDTH * i + j) * 3 + 1] = fmin((int)(255 * c.y), 255);
            png[(WIDTH * i + j) * 3 + 2] = fmin((int)(255 * c.z), 255);
            // png[(WIDTH * i + j) * 4 + 3] = 0.0;
        }
    }

    if(!stbi_write_png("raytraced.png", WIDTH, HEIGHT, 3, png, WIDTH*3)) {
        cout << "ERROR WRITING FILE" << endl;
    }

    free(png);
}
