#include "vec3.h"
#include "Matrix33.h"

Matrix33::Matrix33() { }

double Matrix33::operator()(size_t i, size_t j) const {
    return val[3 * i + j];
}

double& Matrix33::operator()(size_t i, size_t j) {
    return val[3 * i + j];
}

vec3 Matrix33::operator*(vec3 v) const {
    vec3 w(0.0);
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            w[i] += (*this)(i,j) * v[j];
        }
    }

    return w;
}

Matrix33 Matrix33::Identity() {
    Matrix33 mat = Matrix33();
    for(int i = 0; i < 3; i++)
        mat(i, i) = 1.0;

    return mat;
}

Matrix33 Matrix33::BasisRotation(vec3 u, vec3 v, vec3 a, vec3 b) {
    vec3 c = cross(a, b);
    vec3 w = cross(u, v);

    Matrix33 rot;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            rot(i, j) = u[j] * a[i] + v[j] * b[i] + w[j] * c[i];
        }
    }

    return rot;
}

std::ostream& operator<<(std::ostream& os, const Matrix33& mat) {
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            os << mat(i,j) << j == 2 ? ", " : "\n";
    return os;
}