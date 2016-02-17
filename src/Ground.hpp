#ifndef GROUND_H_
#define GROUND_H_

#include "Vector.hpp"
#include "Screen.hpp"
#include "Line.hpp"

// a class to represent collidable surfaces.
// They can either be a pad (which is safe to land on)
// or not a pad (which is not safe to land on)
class Ground : public Line {
    private:
        static const int PAD_HEIGHT = 10;
    public:
        bool is_pad;

        Ground(bool _is_pad, int x1, int y1, int x2, int y2);
        void draw(Screen& s);
};

#endif
