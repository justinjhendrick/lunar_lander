#include "Pilot.hpp"
#include "World.hpp"

Pilot::Pilot(Screen& s) : Lander(s) {
}

void Pilot::fly(World& world) {
    //thrust = max_thrust;
    //pad = world.grounds.at(0);

    //float dist_to_pad = (pad.begin.y - (y_pos + COLLISION_HEIGHT)) /
    //                    pixels_per_meter;

    //float net_accel = g - thrust / (dry_mass + fuel);
    //float thrust_time = fabs(y_vel / net_accel);
    //float thrust_distance = y_vel * thrust_time +
    //    .5 * net_accel * thrust_time * thrust_time;
    ////printf("dt = %f, a = %f, %f <= %f\n", thrust_time, net_accel, dist_to_pad, thrust_distance);

    //if (!thrusting && dist_to_pad <= thrust_distance) {
    //    thrusting = true;
    //}

    //if (thrusting && y_vel * pixels_per_meter < safe_vel * .5) {
    //    thrusting = false;
    //}
}
