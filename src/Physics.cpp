#include "Physics.hpp"
#include "World.hpp"

Physics::Physics(double _x_pos, double _y_pos, double _x_vel, double _y_vel) {
    x_pos = _x_pos;
    y_pos = _y_pos;
    x_vel = _x_vel;
    y_vel = _y_vel;
}

void Physics::move() {
    perturb();
    VelAccel va = next_vel_accel(true);
    x_vel = va.x_vel;
    y_vel = va.y_vel;

    x_pos += (x_vel * DT + .5 * va.x_accel * DT * DT) * PIXELS_PER_METER;
    y_pos += (y_vel * DT + .5 * va.y_accel * DT * DT) * PIXELS_PER_METER;
}

Physics::VelAccel Physics::next_vel_accel(bool real) {
    VelAccel result;
    result.x_accel = 0.;
    result.y_accel = World::g;
    result.x_vel = x_vel;
    result.y_vel = y_vel + result.y_accel * DT;
    return result;
}

void Physics::get_lines(std::vector<Line>& lines_out) {}
void Physics::perturb() {}

Physics::~Physics() {
}
