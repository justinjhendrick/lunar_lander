#include <cmath>
#include <cassert>
#include "../src/Vector.hpp"

void test_ops() {
    Vector a(.2, .6);
    Vector b(1.6, .3);

    assert(fabs(-0.9 - Vector::cross(a, b)) < .001);

    Vector c = Vector::minus(a, b);
    assert(c.x == a.x - b.x);
    assert(c.y == a.y - b.y);

    c = Vector::plus(a, b);
    assert(c.x == a.x + b.x);
    assert(c.y == a.y + b.y);

    assert(fabs(.5 - Vector::dot(a, b)) < .001);
}

void test_segments_intersect() {
    Vector a(1., 1.);
    Vector b(1., 0.);

    Vector c(1.5, 0.);
    Vector d(0., 2.);

    Vector e(0., 0.);

    assert(Vector::segments_intersect(a, b, a, b));
    assert(Vector::segments_intersect(a, b, c, d));
    assert(!Vector::segments_intersect(a, b, e, d));
}

void test_vector() {
    test_ops();
    test_segments_intersect();
}
