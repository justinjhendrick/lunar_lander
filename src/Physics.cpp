#include "Physics.hpp"

Physics::Physics() {}

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
    result.x_vel = 0.;
    result.y_vel = 0.;
    result.x_accel = 0.;
    result.y_accel = 0.;
    return result;
}

void Physics::get_lines(std::vector<Line>& lines_out) {}
void Physics::perturb() {}

Physics::~Physics() {}
