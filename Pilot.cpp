#include <float.h>
#include "Pilot.hpp"
#include "World.hpp"
#include "Lander.hpp"
#include "Ground.hpp"

Pilot::Pilot() {
    state = BEFORE_Y_BURN;
    rot_state = START;
    last_fall_to_pad_vel_prediction = FLT_MAX;
}

// puts difference in 'out'
// returns true if it'll pass through zero
bool Pilot::angle_diff(float a, float b, float* out) {
    float diff = fabs(a - b);
    bool result = false;
    // shouldn't ever rotate more than half circle
    if (diff > M_PI) {
        diff = 2 * M_PI - diff;
        result = true;
    }
    *out = diff;
    return result;
}

// rotate the lander to the target orientation
void Pilot::rotate_to(Lander& l, float tgt_orientation) {
    if (rot_state == START) {
        // choose which direction to rotate
        init_orientation = l.orientation;
        bool through_zero = angle_diff(tgt_orientation,
                                       init_orientation,
                                       &d_theta
        );
        int correction = through_zero ? -1 : 1;

        if (tgt_orientation > l.orientation) {
            l.torque = correction * l.max_torque;
        } else {
            l.torque = -correction * l.max_torque;
        }
        rot_state = TORQUE_UP;
        frame = 1;
    } else if (rot_state == TORQUE_UP) {
        // torque until we make it halfway
        float traveled;
        angle_diff(init_orientation, l.orientation, &traveled);
        if (traveled >= d_theta / 2.) {
            l.torque *= -1;
            rot_state = TORQUE_DOWN;
            flip_frame = frame;
            frame = 1;
        } else {
            frame++;
        }
    } else if (rot_state == TORQUE_DOWN) {
        // null out spin by applying down torque exactly as many frames as we
        // applied up torque
        if (frame == flip_frame) {
            l.torque *= 0;
            rot_state = DONE;
        } else {
            frame++;
        }
    }
}

void Pilot::fly(Lander& l, World& world) {
    l.thrust = l.max_thrust / 5.;

    Ground& pad = world.grounds.at(0);
    float dist_to_pad = (pad.begin.y - (l.y_pos + l.COLLISION_HEIGHT)) /
                        l.pixels_per_meter;
    if (state == BEFORE_Y_BURN) {
        rotate_to(l, M_PI_2);
        // When do we turn on the thruster?
        // What will our acceleration be while thrusting?
        // a = g - F_T / m                 (NOTE: assumption dm/dt << m)
        float net_accel = World::g - l.thrust / (l.dry_mass + l.fuel);
        // How long will that thrust take?
        // t = v / a
        float thrust_time = fabs(l.y_vel / net_accel);
        // How far from pad must we begin burning to burn for t time?
        // d = v * t + (a/2) * t^2
        float thrust_distance = l.y_vel * thrust_time +
            .5 * net_accel * thrust_time * thrust_time;
        if (dist_to_pad <= thrust_distance) {
            l.thrusting = true;
            state = Y_BURN;
        }
    } else if (state == Y_BURN) {
        // When do we turn off the thruster?
        // How fast will I be moving after falling dist_to_pad?
        // v(t) = v_0 + g * t
        // How long will I fall?
        // d = v_0 * t + (g/2) * t^2
        // Solve with quadratic method. Then plug into v(t)
        float a = World::g / 2;
        float b = l.y_vel;
        float c = -dist_to_pad;
        float fall_time = (-b + sqrt(b * b - 4. * a * c)) / (2. * a);
        float vel_at_pad = (l.y_vel + World::g * fall_time) * l.pixels_per_meter;
        if (vel_at_pad < l.safe_vel * .5 ||
                vel_at_pad >= last_fall_to_pad_vel_prediction) {
            l.thrusting = false;
            state = AFTER_Y_BURN;
        }
        last_fall_to_pad_vel_prediction = vel_at_pad;
    }
}
