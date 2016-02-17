#include <cmath>
#include "Line.hpp"

Line::Line(int x1, int y1, int x2, int y2) :
    Line((double) x1, (double) y1, (double) x2, (double) y2) {
}

Line::Line(double x1, double y1, double x2, double y2) :
    begin(fmin(x1, x2), (x1 < x2 ? y1 : y2)),
    segment(fabs(x2 - x1), (x1 < x2 ? (y2 - y1) : (y1 - y2))) {
    // point 1 is left of point 2 (or equal)
}

int Line::get_left() {
    return (int) begin.x;
}
int Line::get_right() {
    return (int) (begin.x + segment.x);
}

int Line::get_center() {
    return (int) (begin.x + segment.x / 2);
}

int Line::get_top() {
    if (segment.y > 0.) {
        return (int) (begin.y + segment.y);
    }
    return (int) begin.y;
}

int Line::get_bot() {
    if (segment.y < 0.) {
        return (int) (begin.y + segment.y);
    }
    return (int) begin.y;
}

bool Line::intersect(Line& l1, Line& l2) {
    // http://stackoverflow.com/questions/563198/
    // how-do-you-detect-where-two-line-segments-intersect
    Vector& p = l1.begin;
    Vector& r = l1.segment;
    Vector& q = l2.begin;
    Vector& s = l2.segment;

    double eps = 0.0001;
    double rs = Vector::cross(r, s);
    Vector qp = Vector::minus(q, p);
    double qpr = Vector::cross(qp, r);
    if (fabs(rs) < eps && fabs(qpr) < eps) {
        // collinear
        double rr = Vector::dot(r, r);
        double t0 = Vector::dot(qp, r) / rr;
        double t1 = Vector::dot(Vector::minus(Vector::plus(q, s), p), r) / rr;
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

    double t = Vector::cross(qp, s) / rs;
    double u = qpr / rs;
    if (0. <= t && t <= 1. && 0. <= u && u <= 1.) {
        return true;
    }
    return false;
}
