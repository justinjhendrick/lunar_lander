#ifndef GROUND_H_
#define GROUND_H_

#include "Lander.hpp"

class Ground {
    public:
        bool is_pad;
        int x1;
        int y1;
        int x2;
        int y2;
        Ground(bool _is_pad, int _x1, int _y1, int _x2, int _y2);
        bool is_colliding(const Lander& l);
};

#endif
