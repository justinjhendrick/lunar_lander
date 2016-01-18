#include "Pilot.hpp"
#include "World.hpp"
#include "Lander.hpp"
#include "Ground.hpp"

Pilot::Pilot() {
}

void Pilot::fly(Lander& l, World& world) {
    l.thrust = l.max_thrust;
    Ground& pad = world.grounds.at(0);

    float dist_to_pad = (pad.begin.y - (l.y_pos + l.COLLISION_HEIGHT)) /
                        l.pixels_per_meter;

    float net_accel = l.g - l.thrust / (l.dry_mass + l.fuel);
    float thrust_time = fabs(l.y_vel / net_accel);
    float thrust_distance = l.y_vel * thrust_time +
        .5 * net_accel * thrust_time * thrust_time;
    //printf("dt = %f, a = %f, %f <= %f\n", thrust_time, net_accel, dist_to_pad, thrust_distance);

    if (!l.thrusting && dist_to_pad <= thrust_distance) {
        l.thrusting = true;
    }

    if (l.thrusting && l.y_vel * l.pixels_per_meter < l.safe_vel * .5) {
        l.thrusting = false;
    }
}
