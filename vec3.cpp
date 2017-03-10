#include "vec3.h"
#include <cmath>
#include <iostream>

vec3::vec3(double v) : x(v), y(v), z(v) { }
vec3::vec3() : vec3(0.0) { }
vec3::vec3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) { }
vec3::vec3(double v[3]) : vec3(v[0], v[1], v[2]) { }

void vec3::operator+=(const vec3 &other) {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
}

vec3 vec3::operator+(const vec3 &v) const {
    return vec3(x + v.x, y + v.y, z + v.z);
}

vec3 vec3::operator-(const vec3 &v) const {
    return vec3(x - v.x, y - v.y, z - v.z);
}

vec3 vec3::operator*(const vec3 &v) const {
    return vec3(x * v.x, y * v.y, z * v.z);
}

vec3 vec3::operator-() const { 
    return vec3(-x, -y, -z); 
}

vec3 vec3::operator*(const double s) const {
    return vec3(x * s, y * s, z * s);
}

vec3 vec3::operator/(const double s) const {
    return vec3(x / s, y / s, z / s);
}

double vec3::operator[](size_t index) const {
    switch(index) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
    }

    throw std::out_of_range("vec3 out of range");
}

double& vec3::operator[](size_t index) {
    switch(index) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
    }

    throw std::out_of_range("vec3 out of range");
}

double vec3::length() const {
    double s = x * x + y * y + z * z;
    return sqrt(s);
}

void vec3::normalize() {
    double length = this->length();
    this->x /= length;
    this->y /= length;
    this->z /= length;
}

vec3 vec3::Spherical(double r, double phi, double theta) {
    return vec3(
        r * sin(phi) * cos(theta),
        r * sin(phi) * sin(theta),
        r * cos(phi)
    );
}

double dot(const vec3 &v, const vec3 &w) {
    return w.x * v.y + w.y * v.y + w.z * v.z;
}

vec3 cross(const vec3 &v, const vec3 &w) {
    return vec3(
        v.y * w.z - v.z * w.y,
        v.z * w.x - v.x * w.z,
        v.x * w.y - v.y * w.x
    );
}

std::ostream& operator<<(std::ostream& os, const vec3& v) {  
    os << "(" << v.x << "," << v.y << "," << v.z << ")";
    return os;  
}  
