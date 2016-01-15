#include "Ground.hpp"
Ground::Ground(bool _is_pad, int _x1, int _y1, int _x2, int _y2) {
    is_pad = _is_pad;
    x1 = _x1;
    y1 = _y1;
    x2 = _x2;
    y2 = _y2;
}

bool Ground::is_colliding(const Lander& l) {
    // TODO
    return false;
}
