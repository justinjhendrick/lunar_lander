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
        unsigned long frame;
        unsigned long flip_frame;

        enum State {
            ROTATION,
            X_BURN1,
            ROTATION2,
            X_BURN2,
            ROTATION3,
            BEFORE_Y_BURN,
            Y_BURN,
            AFTER_Y_BURN
        };
        State state;
        float last_fall_to_pad_vel_prediction;

        bool angle_diff(float a, float b, float* out);
        void rotate_to(Lander& l, float tgt_orientation);
    public:
        Pilot();
        void fly(Lander& l, World& world);
};

#endif
