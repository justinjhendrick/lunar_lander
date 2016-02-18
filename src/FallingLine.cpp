#include "FallingLine.hpp"
#include "Utils.hpp"

FallingLine::FallingLine(int _width,
                         double _x_pos,
                         double _y_pos,
                         double _x_vel,
                         double _y_vel) :
    Physics(_x_pos, _y_pos, _x_vel, _y_vel) {
    width = _width;
}

void FallingLine::get_lines(std::vector<Line>& lines) {
    double orientation = Utils::compute_retrograde(x_vel, y_vel);
    std::pair<double, double> p1;
    std::pair<double, double> p2;
    p1 = Utils::rotate(-width / 2, 0., 3 * M_PI_2 + orientation);
    p2 = Utils::rotate( width / 2, 0., 3 * M_PI_2 + orientation);
    
    //printf("(%f, %f) --- (%f, %f)\n", p1.first + x_pos, p1.second + x_pos,
    //                                  p2.first + x_pos, p2.second + y_pos);
    Line line(p1.first + x_pos, p1.second + y_pos,
              p2.first + x_pos, p2.second + y_pos);
    lines.push_back(line);
}

// for debugging
void FallingLine::print() {
    printf("pos = (%f, %f), vel = (%f, %f)\n", x_pos, y_pos, x_vel, y_vel);
}
