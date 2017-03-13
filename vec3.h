#ifndef VEC3_H
#define VEC3_H

#include <iosfwd>

class vec3 {
    public:
        double x, y, z;

        vec3();
        explicit vec3(double v);
        vec3(double x, double y, double z);
        vec3(double v[3]);

        void operator+=(const vec3 &other);
        bool operator!=(const vec3 &other);
        vec3 operator-() const;

        vec3 operator+(const vec3 &other) const;
        vec3 operator-(const vec3 &other) const;
        vec3 operator*(const vec3 &other) const;

        vec3 operator*(const double s) const;
        vec3 operator/(const double s) const;

        friend vec3 operator*(const double s, const vec3 &v) { return v * s; };

        double operator[](size_t index) const;
        double& operator[](size_t index);

        double length() const;
        void normalize();

        friend vec3 cross(const vec3 &v, const vec3& w);
        friend double dot(const vec3 &v, const vec3& w);
        friend vec3 normalize(const vec3 &v);

        static vec3 Spherical(double r, double phi, double theta);

        friend std::ostream& operator<<(std::ostream& os, const vec3& v);
};

#endif
