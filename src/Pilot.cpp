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
        if (l.vel < 3.) {
            // if we're moving really slowly, retrograde direction
            // isn't very meaningful. Safe to not rotate.
            return;
        }
        //printf("vel = %f, ", l.vel);
        double retrograde = atan2(l.y_vel, l.x_vel);
        //printf("atan2(%f, %f) = %f, ", l.y_vel, l.x_vel, retrograde * 180. / M_PI);
        retrograde += M_PI;
        if (retrograde >= 2 * M_PI) {
            retrograde -= 2 * M_PI;
        }

        double diff;
        if (landing) {
            // don't rotate past the safe difference from straight up
            Utils::angle_diff(retrograde, 3 * M_PI_2, &diff);
            if (diff >= LANDING_ORIENTATION_SAFETY_MARGIN * l.safe_orientation) {
                //printf("retrograde is unsafe for landing!\n");
                if (retrograde > 3 * M_PI_2) {
                    retrograde = 3 * M_PI_2 +
                        LANDING_ORIENTATION_SAFETY_MARGIN * l.safe_orientation;
                } else {
                    retrograde = 3 * M_PI_2 -
                        LANDING_ORIENTATION_SAFETY_MARGIN * l.safe_orientation;
                }
            }

            // smooth retrograde orientation
            smooth_retrograde = alpha * retrograde +
                                (1. - alpha) * smooth_retrograde;
            retrograde = smooth_retrograde;

            //printf(" corrected_retrograde = %f\n", retrograde * 180. / M_PI);
        } else {
            //printf("\n");
            smooth_retrograde = retrograde;
        }

        Utils::angle_diff(retrograde, l.orientation, &diff);
        if (diff > MAX_DIFF_FROM_RETROGRADE) {
            // start a rotation
            target_orientation = retrograde;
            rotate_to(l, target_orientation);
        }
    } else {
        // finish a rotation we started earlier
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
void Pilot::rotate_to(Lander& l, double tgt_orientation) {
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
        double traveled;
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

            double diff;
            Utils::angle_diff(l.orientation, tgt_orientation, &diff);
            //printf("rot error = %f radians\n", diff);
        } else {
            rot_frame++;
        }
    }
}

// y_dist_of_fall in meters (not pixels)
double Pilot::fall_time(double y_vel, double y_dist_of_fall) {
    // d = v_0 * t + (g/2) * t^2
    // Solve with quadratic method
    double a = World::g / 2.;
    double b = y_vel;
    double c = -y_dist_of_fall;
    return (-b + sqrt(b * b - 4. * a * c)) / (2. * a);
}

// the pilot is a state machine.
// see enum Pilot::State in Pilot.hpp for more details
void Pilot::fly(Lander& l, World& world) {
    Ground& pad = world.get_pad();

    // find distance to pad
    int x_pos = Utils::nearest_int(l.x_pos) + l.rot_abt.x;
    int y_pos = Utils::nearest_int(l.y_pos) + l.rot_abt.y;
    double y_dist_to_stop = ((pad.get_top() - STOP_ABOVE_PAD) - y_pos) /
                          l.pixels_per_meter;
    double x_dist_to_stop = (pad.get_center() - x_pos) /
                          l.pixels_per_meter;
    double dist_to_stop = hypot(x_dist_to_stop, y_dist_to_stop);

    if (state == BEGIN) {
        l.thrust = l.max_thrust;
        // Which way do we need to burn to get a trajectory to the pad?
        double fall_t = fall_time(l.y_vel, y_dist_to_stop);
        double x_pos_pred = l.x_pos + l.rot_abt.x +
                           l.x_vel * fall_t * l.pixels_per_meter;
        if (x_pos_pred < pad.get_center() - MAX_XDIST_FROM_PAD_CENTER) {
            state = ROT_HORIZ;
            target_orientation = 0.;
        } else if (x_pos_pred > pad.get_center() + MAX_XDIST_FROM_PAD_CENTER) {
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
        double fall_t = fall_time(l.y_vel, y_dist_to_stop);
        double x_pos_pred = l.x_pos + l.rot_abt.x +
                           l.x_vel * fall_t * l.pixels_per_meter;
        if (fabs(x_pos_pred - pad.get_center()) < MAX_XDIST_FROM_PAD_CENTER) {
            l.thrusting = false;
            state = FALL_TO_PAD;
        }
    } else if (state == FALL_TO_PAD) {
        point_retrograde(l, false);
        // When do we turn on the thruster?
        // What will our acceleration be while thrusting?
        // a = g - F_T / m                 (NOTE: assumption dm/dt << m)
        double y_accel = World::g + l.thrust * sin(l.orientation) /
                                   (l.dry_mass + l.fuel);
        double x_accel = l.thrust * cos(l.orientation) /
                        (l.dry_mass + l.fuel);
        double net_accel = -sqrt(x_accel * x_accel + y_accel * y_accel);
        double vel = hypot(l.x_vel, l.y_vel);
        //printf("fall: hypot(%f, %f) = %f m/s\n", l.x_vel, l.y_vel, vel);
        // How long will that thrust take?
        // t = v / a
        double thrust_time = fabs(vel / net_accel);
        // How far from pad must we begin burning to burn for t time?
        // d = v * t + (a/2) * t^2
        double thrust_distance = vel * thrust_time +
            .5 * net_accel * thrust_time * thrust_time;
        if (dist_to_stop <= thrust_distance) {
            int burn_frames = Utils::nearest_int(thrust_time * 1000. /
                                                 (double) FRAME_TIME);
            l.thrusting = true;
            state = SUICIDE_BURN;
            stop_burn_frame = frame + (unsigned long) burn_frames;
            // above cast to unsigned is safe because
            // burn frames is positive because
            // thrust time is positive because it's a result of fabs()
        }
    } else if (state == SUICIDE_BURN) {
        //double vel = hypot(l.x_vel, l.y_vel);
        //printf("burn: hypot(%f, %f) = %f m/s\n", l.x_vel, l.y_vel, vel);
        point_retrograde(l, false);
        if (frame >= stop_burn_frame) {
            l.thrusting = false;
            state = LAND;
        }
    } else if (state == LAND) {
        //double vel = hypot(l.x_vel, l.y_vel);
        //printf("land: hypot(%f, %f) = %f m/s\n", l.x_vel, l.y_vel, vel);
        point_retrograde(l, true);
        double next_y_vel = l.y_vel + World::g * l.dt;
        double next_vel = hypot(l.x_vel, next_y_vel);
        if (next_vel > l.safe_vel * LANDING_VEL_SAFETY_MARGIN /
                       l.pixels_per_meter) {
            l.thrust = (l.dry_mass + l.fuel) * World::g;
            l.thrusting = true;
        }
    }
    frame++;
}
