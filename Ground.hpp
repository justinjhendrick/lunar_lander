#ifndef GROUND_H_
#define GROUND_H_

#include "Lander.hpp"
#include "Vector.hpp"

class Ground {
    public:
        bool is_pad;
        Vector begin;
        Vector segment;
        Ground(bool _is_pad, int x1, int y1, int x2, int y2);
        bool is_colliding(const Lander& l);
        void draw(Screen& s);
};

#endif
