#include <float.h>
#include "Pilot.hpp"
#include "World.hpp"
#include "Lander.hpp"
#include "Ground.hpp"
#include "constants.hpp"
#include "Utils.hpp"

Pilot::Pilot() {
    state = BEGIN;
    rot_state = START;
    frame = 0;
}

// point craft in direction opposite our velocity ==
// point thrusters in direction of our velocity
//
// the bool flag 'landing' tells us to maintain
// an orientation that's safe for landing
void Pilot::point_retrograde(Lander& l, bool landing) {
    if (rot_state == START) {
        float retrograde = atan2(l.y_vel, l.x_vel);
        retrograde += M_PI;
        if (retrograde >= 2 * M_PI) {
            retrograde -= 2 * M_PI;
        }

        float diff;
        if (landing) {
            // smooth retrograde orientation
            smooth_retrograde = alpha * retrograde +
                                (1. - alpha) * smooth_retrograde;
            retrograde = smooth_retrograde;

            // don't rotate past the safe difference from straight up
            Utils::angle_diff(retrograde, 3 * M_PI_2, &diff);
            if (diff >= l.safe_orientation) {
                if (retrograde > 3 * M_PI_2) {
                    retrograde = 3 * M_PI_2 + .9 * l.safe_orientation;
                } else {
                    retrograde = 3 * M_PI_2 - .9 * l.safe_orientation;
                }
            }
        } else {
            smooth_retrograde = retrograde;
        }

        Utils::angle_diff(retrograde, l.orientation, &diff);
        if (diff > .02) {
            target_orientation = retrograde;
            rotate_to(l, target_orientation);
        }
    } else {
        rotate_to(l, target_orientation);
        if (rot_state == DONE) {
            rot_state = START;
        }
    }
}

// rotate the lander to the target orientation
// to use:
// rotate_to(l, angle)
// if (rot_state == DONE) Go to next state; rot_state = START
void Pilot::rotate_to(Lander& l, float tgt_orientation) {
    if (rot_state == START) {
        // choose which direction to rotate
        init_orientation = l.orientation;
        bool through_zero = Utils::angle_diff(tgt_orientation,
                                              init_orientation,
                                              &d_theta
        );
        int correction = through_zero ? -1 : 1;

        if (tgt_orientation > l.orientation) {
            l.torque = l.max_torque * correction;
        } else {
            l.torque = -l.max_torque * correction;
        }
        rot_state = TORQUE_UP;
        rot_frame = 1;
    } else if (rot_state == TORQUE_UP) {
        // torque until we make it halfway
        float traveled;
        Utils::angle_diff(init_orientation, l.orientation, &traveled);
        if (traveled >= d_theta / 2.) {
            l.torque *= -1;
            rot_state = TORQUE_DOWN;
            flip_frame = rot_frame;
            rot_frame = 1;
        } else {
            rot_frame++;
        }
    } else if (rot_state == TORQUE_DOWN) {
        // null out spin by applying down torque exactly as many frames as we
        // applied up torque
        if (rot_frame >= flip_frame) {
            l.torque = 0;
            rot_state = DONE;

            float diff;
            Utils::angle_diff(l.orientation, tgt_orientation, &diff);
            //printf("rot error = %f radians\n", diff);
        } else {
            rot_frame++;
        }
    }
}

// y_dist_to_pad in m/s (not pixels)
float Pilot::fall_time(float y_vel, float y_dist_to_pad) {
    // d = v_0 * t + (g/2) * t^2
    // Solve with quadratic method
    float a = World::g / 2;
    float b = y_vel;
    float c = -y_dist_to_pad;
    return (-b + sqrt(b * b - 4. * a * c)) / (2. * a);
}

// the pilot is a state machine.
// see enum Pilot::State in Pilot.hpp for more details
void Pilot::fly(Lander& l, World& world) {
    l.thrust = l.max_thrust;

    Ground& pad = world.grounds.at(0);

    // find distance to pad
    int x_pos = l.x_pos + l.rot_abt.x;
    int y_pos = l.y_pos + l.rot_abt.y;
    float y_dist_to_pad = ((pad.begin.y - 15) - y_pos) /
                          l.pixels_per_meter;
    float x_dist_to_pad = (pad.get_center() - x_pos) /
                          l.pixels_per_meter;
    float dist_to_pad = sqrt(x_dist_to_pad * x_dist_to_pad +
                             y_dist_to_pad * y_dist_to_pad);

    if (state == BEGIN) {
        // Which way do we need to burn to get a trajectory to the pad?
        float fall_t = fall_time(l.y_vel, y_dist_to_pad);
        float x_pos_pred = l.x_pos + l.rot_abt.x +
                           l.x_vel * fall_t * l.pixels_per_meter;
        if (x_pos_pred < pad.begin.x + l.rot_abt.x) {
            state = ROT_HORIZ;
            target_orientation = 0.;
        } else if (x_pos_pred > pad.get_right() - l.rot_abt.x) {
            state = ROT_HORIZ;
            target_orientation = M_PI;
        } else {
            state = FALL_TO_PAD;
        }
    } else if (state == ROT_HORIZ) {
        rotate_to(l, target_orientation);
        if (rot_state == DONE) {
            rot_state = START;
            state = X_BURN;
        }
    } else if (state == X_BURN) {
        // thrust until our predicted position is just above the pad
        l.thrusting = true;
        float fall_t = fall_time(l.y_vel, y_dist_to_pad);
        float x_pos_pred = l.x_pos + l.rot_abt.x +
                           l.x_vel * fall_t * l.pixels_per_meter;
        if (x_pos_pred > pad.begin.x + l.rot_abt.x &&
            x_pos_pred < pad.get_right() - l.rot_abt.x) {
            l.thrusting = false;
            state = FALL_TO_PAD;
        }
    } else if (state == FALL_TO_PAD) {
        point_retrograde(l, false);
        // When do we turn on the thruster?
        // What will our acceleration be while thrusting?
        // a = g - F_T / m                 (NOTE: assumption dm/dt << m)
        float y_accel = World::g + l.thrust * sin(l.orientation) /
                                   (l.dry_mass + l.fuel);
        float x_accel = l.thrust * cos(l.orientation) /
                        (l.dry_mass + l.fuel);
        float net_accel = -sqrt(x_accel * x_accel + y_accel * y_accel);
        float vel = sqrt(l.x_vel * l.x_vel + l.y_vel * l.y_vel);
        // How long will that thrust take?
        // t = v / a
        float thrust_time = fabs(vel / net_accel);
        // How far from pad must we begin burning to burn for t time?
        // d = v * t + (a/2) * t^2
        float thrust_distance = vel * thrust_time +
            .5 * net_accel * thrust_time * thrust_time;
        // cuz who needs safety when you can have SPEED
        thrust_distance = thrust_distance / 1.1;
        if (dist_to_pad <= thrust_distance) {
            int burn_frames =
                Utils::round_nearest_int(thrust_time * 1000. /
                                         (float) FRAME_TIME);
            l.thrusting = true;
            state = SUICIDE_BURN;
            stop_burn_frame = frame + burn_frames;
        }
    } else if (state == SUICIDE_BURN) {
        point_retrograde(l, false);
        if (frame >= stop_burn_frame) {
            l.thrusting = false;
            state = LAND;
        }
    } else if (state == LAND) {
        point_retrograde(l, true);
        if (l.y_vel + World::g * l.dt > l.safe_vel / l.pixels_per_meter) {
            l.thrust = (l.dry_mass + l.fuel) * World::g;
            l.thrusting = true;
        }
    }
    frame++;
}
