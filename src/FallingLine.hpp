#ifndef FALLINGLINE_H_
#define FALLINGLINE_H_

#include <vector>
#include "Line.hpp"
#include "Physics.hpp"

// a class used for testing a path for obstacles
class FallingLine : public Physics {
    private:
        int width;
    public:
        FallingLine(int _width,
                    double _x_pos,
                    double _y_pos,
                    double _x_vel,
                    double _y_vel);

        void get_lines(std::vector<Line>& lines) override;

        // for debugging
        void print();
};

#endif
