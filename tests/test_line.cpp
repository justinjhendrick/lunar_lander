#include <cassert>
#include "../src/Line.hpp"

void test_intersect() {
    Line ab(1., 1., 2., 1.);
    Line cd(1.5, 0., 1.5, 2.);
    Line ed(0., 0., 0., 2.);

    assert(Line::intersect(ab, ab));
    assert(Line::intersect(ab, cd));
    assert(!Line::intersect(ab, ed));
}

void test_line() {
    test_intersect();
}
