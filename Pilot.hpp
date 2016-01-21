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
            ROT_HORIZ,      // rotate to horizontal
            X_BURN,         // put us on trajectory to pad
            FALL_TO_PAD,    // and rotate for burn
            SUICIDE_BURN,   // kill velocity
            LAND 
        };
        State state;
        unsigned long frame;
        float target_orientation;
        unsigned long stop_burn_frame;

        void rotate_to(Lander& l, float tgt_orientation);
        void point_retrograde(Lander& l);
        float fall_time(float y_vel, float dist_to_pad);
    public:
        Pilot();
        void fly(Lander& l, World& world);
};

#endif
