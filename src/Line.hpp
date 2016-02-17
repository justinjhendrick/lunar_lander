#ifndef LINE_H_
#define LINE_H_

#include "Vector.hpp"

class Line {
    public:
        Line(int x1, int y1, int x2, int y2);
        Line(double x1, double y1, double x2, double y2);

        // begin points from the origin to the leftmost point
        Vector begin;
        // segment points from the leftmost point to the end of the line
        Vector segment;

        // get the x coordinate of different parts of the line
        int get_left();
        int get_right();
        int get_center();

        // get the y coordinate of different parts of the line
        // if the line is flat, get_top() == get_bot()
        int get_top();
        int get_bot();

        // Does l1 intersect l2?
        static bool intersect(Line& l1, Line& l2);
};

#endif
