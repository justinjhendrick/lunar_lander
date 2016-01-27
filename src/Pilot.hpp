#ifndef PILOT_H_
#define PILOT_H_

#include "World.hpp"
#include "Lander.hpp"

class Pilot {
    private:
        // state variables for rotate_to()
        enum RotationState {
            START,
            TORQUE_UP,
            TORQUE_DOWN,
            DONE
        };
        RotationState rot_state;
        float init_orientation;
        // total angle to rotate
        float d_theta;
        // a count of frames spend on this rotation
        unsigned long rot_frame;
        // the frame that we switch torque directions on
        unsigned long flip_frame;

        // state variables for the landing sequence
        enum State {
            BEGIN,
            ROT_HORIZ,      // rotate to horizontal
            X_BURN,         // put us on trajectory to pad
            FALL_TO_PAD,    // fall and rotate for burn
                            // (actually we target a point just above the pad)
            SUICIDE_BURN,   // kill velocity
            LAND            // descend slowly
        };
        State state;
        // a count of frames flying so far
        unsigned long frame;
        // for use with rotate_to()
        float target_orientation;
        // When to turn off the thrusters
        unsigned long stop_burn_frame;

        // exponential smoothing of retrograde orientation while landing
        float smooth_retrograde;
        const float alpha = .5;

        // constants
        const int STOP_ABOVE_PAD = 15; // pixels
        const float LANDING_ORIENTATION_SAFETY_MARGIN = 0.9;
        const float MAX_DIFF_FROM_RETROGRADE = .02; // radians
        const float LANDING_VEL_SAFETY_MARGIN = .95; // for rounding error

        // rotate the lander to the tgt_orientation
        void rotate_to(Lander& l, float tgt_orientation);

        // point the thrusters in the direction of travel
        void point_retrograde(Lander& l, bool landing);

        // how long will a freefall of y_fall_dist take, starting at y_vel?
        float fall_time(float y_vel, float y_fall_dist);
    public:
        Pilot();
        void fly(Lander& l, World& world);
};

#endif
