#ifndef LANDER_H_
#define LANDER_H_

#include <stdint.h>
#include "Screen.hpp"

class Lander {
    private:
        // initialized by constructor
        float x_pos; // meters
        float y_pos;
        float x_vel; // meters/frame
        float y_vel;
        float orientation; // radians cw from x axis
        float spin_rate; // radians/frame. positive is cw, negative is ccw
        float max_torque; // radians/frame^2 (const)
        float fuel; // kg
        float dry_mass; // kg (const)
        float init_fuel; // kg (const)
        float thrust; // Newtons
        float max_thrust; // Newtons (const)

        bool thrusting = false;
        static const int WIDTH = 21;
        static const int HEIGHT = 40;
        static const int COLLISION_HEIGHT = 30;
        float torque;
        SDL_Texture* txtr;
        SDL_Texture* txtr_fire_low;
        SDL_Texture* txtr_fire_med;
        SDL_Texture* txtr_fire_high;

        SDL_Texture* load_texture(SDL_Renderer* r, const char filename[]);
    public:
        Lander(Screen& s);
        Lander(Screen& s,
               float _x_pos,
               float _y_pos,
               float _x_vel,
               float _y_vel,
               float _orientation,
               float _spin_rate,
               float _max_torque,
               float _fuel,
               float _dry_mass,
               float _init_fuel,
               float _thrust,
               float _max_thrust
        );
        ~Lander();

        void draw(Screen& s);
        void handle(SDL_Event* e);
        void move();
};

#endif
