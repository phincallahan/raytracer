#include <iostream>
#include <cmath>
#include "CImg.h"

using namespace std;

class vec3 
{
    public:
        double x, y, z;
        vec3(double xCo, double yCo, double zCo) : 
            x(xCo), y(yCo), z(zCo) { }

        vec3 operator+(const vec3 &other) {
            return vec3(other.x + this->x, other.y + this->y, other.z + this->z);
        }

        vec3 operator-(const vec3 &other) {
            return vec3(other.x - this->x, other.y - this->y, other.z - this->z);
        }

        double dot(vec3 &other) {
            return this->x * other.x + this->y * other.y + this->z * other.z;
        }

        double length() {
            double s = this->x * this->x + this->y * this->y + this->z * this->z;
            return sqrt(s);
        }

        vec3 normalize() {
            double length = this->length();
            return vec3(this->x / length, this->y / length, this->z / length);
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

    private:
        double val[16];
};

class Ray 
{
    public:
        vec3 origin, dir;
        Ray(vec3 o, vec3 d) : origin(o), dir(d.normalize()) { }
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
        virtual bool intersect(Ray) const = 0;
        // virtual ~Shape();
};

class Sphere : public Shape {
    public:
        vec3 center;
        double radius;
        Sphere(vec3 c, double r) : center(c), radius(r) { }
        bool intersect(Ray ray) const {
            vec3 diff = ray.origin - this->center;
            double c = ray.dir.dot(diff);
            double l = diff.length();
            double d = c * c - l * l + this->radius * this->radius;
            return d >= 0.0;
        }
};

const int WIDTH = 512;
const int HEIGHT = 512;

int main() {
    vec3 origin (0.0, 0.0, 0.0);
    vec3 dir (0.75, 0.25, 0.0);
    Ray ray (origin, dir);

    cimg_library::CImg<unsigned char> img(WIDTH, HEIGHT, 1, 3);
    img.fill(0.0);
    for(int x = 0; x < WIDTH; x++) {
        for(int y = 0; y < HEIGHT; y++) {
            unsigned char color[3] = { 255 * rand(), 255 * rand(), 255 * rand() };
            img.draw_point(x,y, color);
        }
    }

    img.save("test_trace.png");

    vec3 center(2.0, 0.0, 0.0);
    Sphere sphere(center, 1.0);

    Matrix44 mat = Matrix44::identity();


    cout << sphere.intersect(ray) << "\n";
}