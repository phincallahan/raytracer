#include <iostream>
#include <cmath>
#include "CImg.h"
#include <vector>

using namespace std;

class vec3 
{
    public:
        double x, y, z;
        vec3(double xCo, double yCo, double zCo) : 
            x(xCo), y(yCo), z(zCo) { }

        vec3 operator+(const vec3 &other) const {
            return vec3(other.x + this->x, other.y + this->y, other.z + this->z);
        }

        vec3 operator-(const vec3 &other) const {
            return vec3(this->x - other.x, this->y - other.y, this->z - other.z);
        }

        vec3 operator-() const {
            return vec3(-this->x, -this->y, -this->z);
        }

        vec3 operator*(const double scale) const {
            return vec3(this->x * scale, this->y * scale, this->z * scale);
        }

        vec3 operator*(const vec3 &other) const {
            return vec3(this->x * other.x, this->y * other.y, this->z * other.z);
        }

        double dot(const vec3 &other) const {
            return this->x * other.x + this->y * other.y + this->z * other.z;
        }

        double length() {
            double s = this->x * this->x + this->y * this->y + this->z * this->z;
            return sqrt(s);
        }

        void normalize() {
            double length = this->length();
            this->x /= length;
            this->y /= length;
            this->z /= length;
        }

        void print() {
            cout << "(" << this->x
                 << ", " << this->y
                 << ", " << this->z
                 << ")\n";
        }
};

class Matrix44 
{
    public: 
        Matrix44() {
            for(int i = 0; i < 16; i++) val[i] = 0.0;
        }

        void set(int i, int j, double v) {
            val[i*4 + j] = v;
        }

        double get(int i , int j) {
            return val[i * 4 + j];
        }

        void print() {
            for(int i = 0; i < 4; i++) {
                for(int j = 0; j < 4; j++) {
                    cout << this->get(i,j) << " ";
                }
                cout << "\n";
            }
        }

        static Matrix44 identity() {
            Matrix44 mat = Matrix44();
            for(int i = 0; i < 4; i++)
                mat.set(i, i, 1.0);

            return mat;
        }

        vec3 multiply(vec3 v, double end) {
            vec3 ret = vec3(0.0, 0.0, 0.0);
            ret.x = v.x * val[0] + v.y * val[1] + v.z * val[2] + end * val[3];
            ret.y = v.x * val[4] + v.y * val[5] + v.z * val[6] + end * val[7];
            ret.z = v.x * val[8] + v.y * val[9] + v.z * val[10] + end * val[11];

            return ret;
        }

    private:
        double val[16];
};

class Ray 
{
    public:
        vec3 origin, dir;
        Ray(vec3 o, vec3 d) : origin(o), dir(d) { dir.normalize(); }
        
        vec3 getPoint(const double t) const {
            return this->origin + this->dir * t;
        }

        void print() {
            cout << "Origin: ";
            this->origin.print();
            cout << "Direction: ";
            this->dir.print();
        }
};

class Shape 
{
    public:
        virtual bool intersect(Ray, double *) const = 0;
        virtual vec3 normal(const vec3) const = 0;
        virtual vec3 colorAt(const vec3) const = 0;
        // virtual ~Shape();
};

class Sphere : public Shape {
    public:
        vec3 center, color;
        double radius;
        Sphere(vec3 pos, vec3 col, double r) : center(pos), color(col), radius(r) { }

        //https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
        bool intersect(Ray ray, double *t) const {
            vec3 diff = ray.origin - this->center;
            double c = diff.length();
            double b = ray.dir.dot(diff);
            double d = b * b - c * c + this->radius * this->radius;

            if(d < 0) {
                *t = -1;
                return false;
            }

            double t1 = -b + sqrt(d);
            double t2 = -b - sqrt(d);

            if(t1 < 0 && t2 < 0) {
                return false;
            }

            if(t1 > 0 && t2 > 0) {
                *t = fmin(t1, t2);
            } else {
                if (t1 > 0) {
                    *t = t1;
                } else {
                    *t = t2;
                }
            }

            return true;
        }

        vec3 normal(const vec3 surfacePoint) const {
            vec3 normal = surfacePoint - this->center;
            normal.normalize();
            return normal;
        }

        vec3 colorAt(const vec3 surfacePoint) const {
            return this->color;
        }
};

class Light {
    public: 
        vec3 color, pos;
        Light(vec3 p, vec3 col) : pos(p), color(col) { }
};

const int WIDTH = 2048;
const int HEIGHT = 2048;

Shape* findIntersect(const Ray &ray, vector<Shape *> &shapes, double *t_min) {
    *t_min = -1;
    double t = 0;
    Shape * closest_shape;
    for(int i = 0; i < shapes.size(); i++) {
        if(shapes[i]->intersect(ray, &t))  {
            if(t > 0 && (*t_min < 0 || *t_min > t)) {
                *t_min = t;
                closest_shape = shapes[i];
            } 
        }
    }

    return closest_shape;
}

// TODO: BUNDLE SHAPES AND LIGHTS AND CAM POS
vec3 localLighting(const vec3 &intersect, const vec3 &normal, 
                   const vec3 &camPos, 
                   vector<Shape *> &shapes,
                   vector<Light *> &lights) {
    double intensity = 0.0, t;
    vec3 camDir = camPos - intersect;
    camDir.normalize();

    for(int i = 0; i < lights.size(); i++) {
        vec3 lightDir = lights[i]->pos - intersect;
        lightDir.normalize();

        if(normal.dot(lightDir) < 0) 
            continue;

        Ray shadowRay (intersect + lightDir * .000001, lightDir);
        findIntersect(shadowRay, shapes, &t);

        if (t >= 0) 
            continue;

        vec3 reflected = normal * 2 * normal.dot(lightDir) - lightDir;
        reflected.normalize();

        // Diffuse lighting
        intensity += lightDir.dot(normal);
        // Specular lighting
        intensity += pow(camDir.dot(reflected), 32.0);
    }

    intensity = fmax(0.1, intensity);
    intensity = fmin(1.0, intensity);

    // TODO: TAKE INTO ACCOUNT COLOR OF LIGHTS
    return vec3(intensity, intensity, intensity);
}

vec3 trace(const Ray &ray, const vec3 &cameraPos, 
           vector<Shape *> &shapes, 
           vector<Light *> &lights) {
    double t;
    Shape * closest_shape = findIntersect(ray, shapes, &t);
    if(t < 0) {
        return vec3(0.0, 0.0, 0.0);
    } else {
        vec3 intersect = ray.getPoint(t);
        vec3 normal = closest_shape->normal(intersect);

        vec3 local = localLighting(intersect, normal, cameraPos, shapes, lights);
        vec3 color = closest_shape->colorAt(intersect) * local;
        return color;
    }
}

int main() {
    cimg_library::CImg<double> img(WIDTH, HEIGHT, 1, 3);
    img.fill(0.0);

    Sphere sphere1(vec3(0.0, 0.0, -2.5), vec3(1.0, 0.0, 0.0), 1.0);
    Sphere sphere2(vec3(-0.25, 0.0, -1.0), vec3(0.0, 1.0, 0.0), .25);

    vector<Shape *> shapes(2);
    shapes[0] = &sphere1;
    shapes[1] = &sphere2;

    Light light(vec3(-1.0, 1.0, 0.0), vec3(1.0, 1.0, 1.0));
    vector<Light *> lights(1);
    lights[0] = &light;

    // TODO: ADD IN CODE THAT PRODUCES 4x4 FROM ROTATION
    // TODO: CREATE A 3x3 MATRIX CLASS
    // TODO: PORT AXIS ROTATION
    vec3 cameraPos = vec3(0.0, 0.0, 0.0);
    Matrix44 cameraMatrix = Matrix44::identity();

    double scale = tan(M_PI / 3);

    //Generate the initial rays. One for each pixel in the screen.
    for(int i = 0; i < WIDTH; i++) {
        for(int j = 0; j < HEIGHT; j++) {
            double x = (2 * (i + 0.5) / (double) WIDTH - 1) *  scale; 
            double y = (1 - 2 * (j + 0.5) / (double) HEIGHT) * scale; 

            vec3 dir = cameraMatrix.multiply(vec3(x, y, -1), 1.0);
            dir.normalize();
            Ray ray (vec3(0.0, 0.0, 0.0), dir);

            vec3 c = trace(ray, cameraPos, shapes, lights);
            double colors[3] = { c.x, c.y, c.z };
            img.draw_point(i, j, colors);
        }
    }

    img.normalize(0, 255);
    img.save("test_trace.png");
}