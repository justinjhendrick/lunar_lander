#ifndef GROUND_H_
#define GROUND_H_

#include "Vector.hpp"
#include "Screen.hpp"

class Ground {
    public:
        bool is_pad;
        Vector begin;
        Vector segment;
        Ground(bool _is_pad, int x1, int y1, int x2, int y2);
        void draw(Screen& s);
        int get_right();
        int get_center();
};

#endif
