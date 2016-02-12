#include <cassert>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../src/Utils.hpp"

void test_angle_diff() {
    double out = 0.0;
    assert(Utils::abs_angle_diff(.1, 2 * M_PI - .1, &out));
    assert(fabs(out - .2) < .001);

    out = 0.0;
    assert(!Utils::abs_angle_diff(M_PI + .1, M_PI - .1, &out));
    assert(fabs(out - .2) < .001);

    // TODO: test non-absolute version
}

void test_nearest_int() {
    assert(0 == Utils::nearest_int(-0.1));
    assert(0 == Utils::nearest_int(0.0));
    assert(0 == Utils::nearest_int(0.1));

    assert(1 == Utils::nearest_int(0.9));
    assert(1 == Utils::nearest_int(0.8));
    assert(1 == Utils::nearest_int(0.7));
    assert(1 == Utils::nearest_int(1.0));
    assert(1 == Utils::nearest_int(1.1));
    assert(1 == Utils::nearest_int(1.2));
    assert(1 == Utils::nearest_int(1.3));
    assert(1 == Utils::nearest_int(1.4));

    assert(2 == Utils::nearest_int(1.5));
    assert(2 == Utils::nearest_int(1.6));
}

void test_random() {
    Utils::seed_random(time(NULL));
    double f = Utils::rand_double(0., 10.);
    assert(f >= 0. && f <= 10.);

    int i = Utils::rand_int(1, 10);
    assert(i >= 0 && i <= 10);
}

void test_utils() {
    test_angle_diff();
    test_nearest_int();
    test_random();
}
