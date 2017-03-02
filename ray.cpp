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

        vec3 operator*(const double scale) {
            return vec3(this->x * scale, this->y * scale, this->z * scale);
        }

        double dot(vec3 &other) {
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
        
        vec3 getPoint(double t) {
            return origin + dir * t;
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
        // virtual ~Shape();
};

class Sphere : public Shape {
    public:
        vec3 center;
        double radius;
        Sphere(vec3 c, double r) : center(c), radius(r) { }

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

            double t1 = b + sqrt(d);
            double t2 = b - sqrt(d);

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

        vec3 normal(vec3 surfacePoint) {
            vec3 normal = this->center - surfacePoint;
            normal.normalize();
            return normal;
        }
};

const int WIDTH = 2048;
const int HEIGHT = 2048;

int main() {
    cimg_library::CImg<char> img(WIDTH, HEIGHT, 1, 1);
    img.fill(0.0);

    vec3 center(0.0, 0.0, -2.0);
    Sphere sphere(center, 1.0);

    Matrix44 mat = Matrix44::identity();

    double scale = tan(M_PI / 4);
    for(int i = 0; i < WIDTH; i++) {
        for(int j = 0; j < HEIGHT; j++) {
            double x = (2 * (i + 0.5) / (double) WIDTH - 1) *  scale; 
            double y = (1 - 2 * (j + 0.5) / (double) HEIGHT) * scale; 

            vec3 dir = mat.multiply(vec3(x, y, -1), 1.0);
            dir.normalize();
            Ray ray (vec3(0.0, 0.0, 0.0), dir);
            double t;

            if(sphere.intersect(ray, &t)) {
                char color[1] = { (char)floor(255 * t/2.0) };
                img.draw_point(i, j, color);
            }
        }
    }

    img.save("test_trace.png");
}