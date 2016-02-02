#include "Vector.hpp"
#include <cstdio>
#include <cmath>

Vector::Vector(double _x, double _y) {
    x = _x;
    y = _y;
}

double Vector::cross(const Vector& a, const Vector& b) {
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

double Vector::dot(const Vector& a, const Vector& b) {
    return a.x * b.x + a.y * b.y;
}

bool Vector::segments_intersect(const Vector& p,
                                const Vector& r,
                                const Vector& q,
                                const Vector& s) {
    double eps = 0.0001;
    double rs = cross(r, s);
    Vector qp = minus(q, p);
    double qpr = cross(qp, r);
    if (fabs(rs) < eps && fabs(qpr) < eps) {
        // collinear
        double rr = dot(r, r);
        double t0 = dot(qp, r) / rr;
        double t1 = dot(minus(plus(q, s), p), r) / rr;
        if (t0 > t1) {
            // make [t0, t1] well formed
            double tmp = t0;
            t0 = t1;
            t1 = tmp;
        }
        // does [t0, t1] intersect [0, 1]?
        if (t0 <= 1. && t1 >= 0.) {
            // collinear and overlapping
            return true;
        }
        // collinear but disjoint
        return false;
    } else if (fabs(rs) < eps) {
        // parallel
        return false;
    }

    double t = cross(qp, s) / rs;
    double u = qpr / rs;
    if (0. <= t && t <= 1. && 0. <= u && u <= 1.) {
        return true;
    }
    return false;
}
