#ifndef PILOT_H_
#define PILOT_H_

#include "World.hpp"
#include "Lander.hpp"

class Pilot {
    private:
        enum RotationState {
            START,
            TORQUE_UP,
            TORQUE_DOWN,
            DONE
        };
        RotationState rot_state;
        float init_orientation;
        float d_theta;
        unsigned long rot_frame;
        unsigned long flip_frame;

        enum State {
            BEGIN,
            ROTATION1,     // rotate horizontal
            INITIATE_X_BURN1,
            X_BURN1,       // go toward pad
            ROTATION2,     // rotate to other horizontal (may need to skip)
            INITIATE_X_BURN2,
            X_BURN2,       // kill x velocity (may need to skip)
            ROTATION3,     // point thruster down
            BEFORE_Y_BURN, // fall and wait
            Y_BURN,        // kill y velocity
            AFTER_Y_BURN   // touchdown softly
        };
        State state;
        unsigned long frame;
        const float PI_FLIP_TIME = 1.58; // seconds. TODO: don't hardcode this
        float target_orientation;
        unsigned long stop_burn_frame;

        void rotate_to(Lander& l, float tgt_orientation);
        float fall_time(float y_vel, float dist_to_pad);
    public:
        Pilot();
        void fly(Lander& l, World& world);
};

#endif
