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
