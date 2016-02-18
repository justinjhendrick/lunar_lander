#include "Pilot.hpp"
#include "World.hpp"
#include "Lander.hpp"
#include "Ground.hpp"
#include "constants.hpp"
#include "Utils.hpp"
#include "PID.hpp"
#include "FallingLine.hpp"

Pilot::Pilot() :
    pid(DT, Lander::MAX_TORQUE, -Lander::MAX_TORQUE, PID_KP, PID_KD, PID_KI) {
    state = BEGIN;
    rot_state = START;
    frame = 0;
}

// point craft in direction opposite our velocity ==
// point thrusters in direction of our velocity
//
// This should be called after we decide on thrusting for this frame.
// That way, next_velocity is correct.
void Pilot::point_retrograde(Lander& l, bool landing) {
    std::pair<double, double> next_vel = l.next_velocity();
    double next_retro = Utils::compute_retrograde(next_vel.first,
                                                  next_vel.second);
    // calculate(desired, current)
    double pid_output = pid.calculate(next_retro, l.orientation);
    //printf("%f, %f, %f\n", next_retro, l.orientation, pid_output);
    l.torque = pid_output;
}

// rotate the lander to the target orientation
// to use:
// rotate_to(l, angle)
// if (rot_state == DONE) Go to next state; rot_state = START
void Pilot::rotate_to(Lander& l, double tgt_orientation) {
    if (rot_state == START) {
        // choose which direction to rotate
        init_orientation = l.orientation;
        bool through_zero = Utils::abs_angle_diff(tgt_orientation,
                                                  init_orientation,
                                                  &d_theta
        );
        int correction = through_zero ? -1 : 1;

        if (tgt_orientation > l.orientation) {
            l.torque = Lander::MAX_TORQUE * correction;
        } else {
            l.torque = -Lander::MAX_TORQUE * correction;
        }
        rot_state = TORQUE_UP;
        rot_frame = 1;
    } else if (rot_state == TORQUE_UP) {
        // torque until we make it halfway
        // then, switch torque direction
        double traveled;
        Utils::abs_angle_diff(init_orientation, l.orientation, &traveled);
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
            Utils::abs_angle_diff(l.orientation, tgt_orientation, &diff);
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

// checks our current ballistic trajectory for obstacles
// returns true if we will hit something that is not the pad
bool Pilot::predict_fall(Lander& l, World& world) {
    FallingLine path_tester(Lander::WIDTH,
                            l.x_pos + Lander::WIDTH / 2,
                            l.y_pos + Lander::COLLISION_HEIGHT,
                            l.x_vel, l.y_vel);
    World::CollisionResult cr = World::NO_COLLISION;
    //double pad = world.get_pad().get_top();
    while (cr == World::NO_COLLISION) {// && fabs(pad - path_tester.y_pos
        path_tester.move();
        cr = world.check_collision(path_tester);
    }
    if (cr == World::PAD) {
        return false;
    } else {
        path_tester.print();
        return true;
    }
}

// the pilot is a state machine.
// see enum Pilot::State in Pilot.hpp for more details
void Pilot::fly(Lander& l, World& world) {
    Ground& pad = world.get_pad();

    // find distance to pad
    int x_pos = Utils::nearest_int(l.x_pos) + l.rot_abt.x;
    int y_pos = Utils::nearest_int(l.y_pos) + l.rot_abt.y;
    double y_dist_to_stop = ((pad.get_top() - STOP_ABOVE_PAD) - y_pos) /
                          Physics::PIXELS_PER_METER;
    double x_dist_to_stop = (pad.get_center() - x_pos) /
                          Physics::PIXELS_PER_METER;
    double dist_to_stop = hypot(x_dist_to_stop, y_dist_to_stop);

    if (state == BEGIN) {
        l.thrust = l.max_thrust;
        // Which way do we need to burn to get a trajectory to the pad?
        double fall_t = fall_time(l.y_vel, y_dist_to_stop);
        double x_pos_pred = l.x_pos + l.rot_abt.x +
                           l.x_vel * fall_t * Physics::PIXELS_PER_METER;
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
                           l.x_vel * fall_t * Physics::PIXELS_PER_METER;
        if (fabs(x_pos_pred - pad.get_center()) < MAX_XDIST_FROM_PAD_CENTER) {
            l.thrusting = false;
            bool obstacle_ahead = predict_fall(l, world);
            printf("obstacle_ahead = %d\n", obstacle_ahead);
            if (obstacle_ahead) {
                state = AVOID;
            } else {
                state = FALL_TO_PAD;
            }
        }
    } else if (state == AVOID) {
        // This doesn't work. It avoids the first obstacle,
        // but never turns off the thruster.
        rotate_to(l, 3 * M_PI_2);
        if (rot_state == DONE) {
            l.thrusting = true;
            bool obstacle_ahead = predict_fall(l, world);
            if (!obstacle_ahead) {
                // After we dodge the obstacle,
                // we're on a new path that doesn't intersect the pad.
                // So, the code below never runs.
                l.thrusting = false;
                rot_state = START;
                state = FALL_TO_PAD;
            }
        }
    } else if (state == FALL_TO_PAD) {
        // When do we turn on the thruster?
        // What will our acceleration be while thrusting?
        // a = g - F_T / m                 (NOTE: assumption dm/dt << m)
        double y_accel = World::g + l.thrust * sin(l.orientation) /
                                   (l.dry_mass + l.fuel);
        double x_accel = l.thrust * cos(l.orientation) /
                        (l.dry_mass + l.fuel);
        double net_accel = -sqrt(x_accel * x_accel + y_accel * y_accel);
        double vel = hypot(l.x_vel, l.y_vel);
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
        point_retrograde(l, false);
    } else if (state == SUICIDE_BURN) {
        if (frame >= stop_burn_frame) {
            l.thrusting = false;
            state = LAND;
        }
        point_retrograde(l, false);
    } else if (state == LAND) {
        // if our next velocity is unsafe,
        // accelerate at World::g to stay safe
        double next_y_vel = l.y_vel + World::g * DT;
        double next_vel = hypot(l.x_vel, next_y_vel);
        if (next_vel > Lander::SAFE_VEL * LANDING_VEL_SAFETY_MARGIN /
                       Physics::PIXELS_PER_METER) {
            l.thrust = (l.dry_mass + l.fuel) * World::g;
            l.thrusting = true;
        }
        point_retrograde(l, true);
    }
    frame++;
}
