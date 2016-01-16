#ifndef LANDER_H_
#define LANDER_H_

#include <stdint.h>
#include "Screen.hpp"

class Lander {
    private:
        // initialized by constructor
        float x_pos; // meters
        float y_pos;
        float x_vel; // meters/second
        float y_vel;
        float orientation; // radians cw from x axis
        float spin_rate; // radians/frame. positive is cw, negative is ccw
        float max_torque; // radians/frame^2 (const)
        float fuel; // kg
        float dry_mass; // kg (const)
        float init_fuel; // kg (const)
        float thrust; // Newtons
        float max_thrust; // Newtons (const)
        float exhaust_vel; // m/s

        bool thrusting = false;
        float torque;
        float dt;
        float vel; // magnitude
        const float safe_vel = 10.;
        const float max_vel = 100.; // only cosmetic
        SDL_Texture* txtr;
        SDL_Texture* txtr_fire_low;
        SDL_Texture* txtr_fire_med;
        SDL_Texture* txtr_fire_high;
        SDL_Texture* fuel_txtr;
        SDL_Texture* thrust_txtr;
        SDL_Texture* vel_txtr;

        void craft_to_sdl_coords();
        void draw_status(Screen& s);
    public:
        static const int WIDTH = 21;
        static const int HEIGHT = 40;
        static const int COLLISION_HEIGHT = 30;

        // collision points of triangle
        // in sc coords (rot_abt is center)
        // Looking at the SC right side up
        // 1 is bottom left
        // 2 is top
        // 3 is bottom right
        const float sc_p1x = -WIDTH / 2.;
        const float sc_p1y = 0.;
        const float sc_p2x = 0.;
        const float sc_p2y = (float) COLLISION_HEIGHT;
        const float sc_p3x = WIDTH / 2.;
        const float sc_p3y = 0.;
        float p1x;
        float p1y;
        float p2x;
        float p2y;
        float p3x;
        float p3y;

        Lander(Screen& s);
        Lander(Screen& s,
               float _x_pos,
               float _y_pos,
               float _x_vel,
               float _y_vel,
               float _orientation,
               float _spin_rate,
               float _max_torque,
               float _dry_mass,
               float _init_fuel,
               float _thrust,
               float _max_thrust,
               float _exhaust_vel
        );
        ~Lander();

        void draw(Screen& s);
        void handle(SDL_Event* e);
        void move();
        bool safe_speed();
};

#endif
