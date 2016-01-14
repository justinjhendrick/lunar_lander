#ifndef LANDER_H_
#define LANDER_H_

#include <stdint.h>
#include "Screen.hpp"

class Lander {
    private:
        float x_pos; // meters
        float y_pos;
        float x_vel; // meters/frame
        float y_vel;
        float orientation; // radians ccw from x axis
        float spin_rate; // radians/frame. positive is ccw, negative is cw
        float torque; // radians/frame^2 (const)
        float fuel; // kg
        float dry_mass; // kg (const)
        float init_fuel; // kg (const)
        float thrust; // Newtons
        float max_thrust; // Newtons (const)
    public:
        Lander();
        Lander(float _x_pos,
               float _y_pos,
               float _x_vel,
               float _y_vel,
               float _orientation,
               float _spin_rate,
               float _torque,
               float _fuel,
               float _dry_mass,
               float _init_fuel,
               float _thrust,
               float _max_thrust
        );
        //~Lander();

        void draw(Screen& s);
};

#endif
