#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <vector>
#include "constants.hpp"
#include "Line.hpp"

// a physical object that can move
// and optionally collide with things
class Physics {
    protected:
        double x_pos;       // pixels. positive is to the right on the screen.
        double y_pos;       // positive is down the screen.
        double x_vel;       // meters/second
        double y_vel;

        typedef struct VelAccel {
            double x_vel;
            double y_vel;
            double x_accel;
            double y_accel;
        } VelAccel;
        
    public:
        Physics();
        virtual ~Physics();
        void move();

        // computes next velocity (m/s) and and acceleration (m/s^2)
        //
        // if [real] is true,
        // the object may change state
        //
        // if [real] is false,
        // this is only a prediction and the object may not change its state.
        virtual VelAccel next_vel_accel(bool real);

        // fill lines array for collision checking
        // Put nothing in lines to make an uncollidable object
        virtual void get_lines(std::vector<Line>& lines_out);

        virtual void perturb();

        static const int PIXELS_PER_METER = 10;
};

#endif
