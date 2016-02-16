#ifndef GROUND_H_
#define GROUND_H_

#include "Vector.hpp"
#include "Screen.hpp"

// a class to represent collidable surfaces.
// They can either be a pad (which is safe to land on)
// or not a pad (which is not safe to land on)
class Ground {
    private:
        int PAD_HEIGHT = 10;
    public:
        bool is_pad;
        // begin points from the origin to the leftmost point
        Vector begin;
        // segment points from the leftmost point to the end of the line
        Vector segment;

        // point 1 must be left of point 2
        Ground(bool _is_pad, int x1, int y1, int x2, int y2);
        void draw(Screen& s);

        // get the x coordinate of different parts of the line
        int get_left();
        int get_right();
        int get_center();

        // get the y coordinate of different parts of the line
        // if the line is flat, get_top() == get_bot()
        int get_top();
        int get_bot();
};

#endif
