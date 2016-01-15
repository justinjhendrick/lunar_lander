#include "Vector.hpp"
#include <cstdio>
#include <cmath>

Vector::Vector(float _x, float _y) {
    x = _x;
    y = _y;
}

float Vector::cross_mag(const Vector& a, const Vector& b) {
    return a.x * b.y - a.y * b.x;
}

Vector Vector::minus(const Vector & a, const Vector& b) {
    Vector result(a.x - b.x, a.y - b.y);
    return result;
}

Vector Vector::plus(const Vector & a, const Vector& b) {
    Vector result(a.x + b.x, a.y + b.y);
    return result;
}

bool Vector::segments_intersect(const Vector& p,
                                const Vector& r,
                                const Vector& q,
                                const Vector& s) {
    float eps = 0.0001;
    float rs = cross_mag(r, s);
    Vector qp = minus(q, p);
    float qpr = cross_mag(qp, r);
    if (fabs(rs) < eps && fabs(qpr) < eps) {
        // collinear
        return true;
    } else if (fabs(rs) < eps) {
        // parallel
        return false;
    }

    float t = cross_mag(qp, s) / rs;
    float u = qpr / rs;
    if (0. <= t && t <= 1. && 0. <= u && u <= 1.) {
        return true;
    }
    return false;
}
