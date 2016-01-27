#ifndef LANDER_H_
#define LANDER_H_

#include <stdint.h>
#include "Screen.hpp"
#include "Ground.hpp"
#include "World.hpp"

class Pilot;

class Lander {
    private:
        // initialized by constructor
        float x_pos;       // pixels. positive is to the right on the screen.
        float y_pos;       // positive is down the screen.
        float x_vel;       // meters/second
        float y_vel;
        float orientation; // radians cw from x axis
        float spin_rate;   // radians/second. positive is cw, negative is ccw
        float max_torque;  // radians/second^2 (const).
                           // Torque is misnomer.
                           // it's actually angular acceleration.
        float fuel;        // kg
        float dry_mass;    // kg (const)
        float init_fuel;   // kg (const)
        float thrust;      // Newtons
        float max_thrust;  // Newtons (const)
        float exhaust_vel; // m/s

        bool thrusting = false;
        float torque;                            // radians/s^2.
                                                 // torque is misnomer.
        float dt;                                // seconds
        float vel;                               // magnitude in pixels/s
        const float safe_vel = 15.;              // pixels/s
        const float max_vel = 100.;              // pixels/s. only cosmetic
        const float safe_orientation = M_PI / 8; // radians from 3 * PI / 2
        const int pixels_per_meter = 10;         // pixels/meter

        // image textures
        SDL_Texture* txtr;
        SDL_Texture* txtr_fire_low;
        SDL_Texture* txtr_fire_med;
        SDL_Texture* txtr_fire_high;
        SDL_Texture* txtr_explosion;
        SDL_Texture* txtr_torque_cw;
        SDL_Texture* txtr_torque_ccw;
        // text textures
        SDL_Texture* fuel_txtr;
        SDL_Texture* thrust_txtr;
        SDL_Texture* vel_txtr;
        SDL_Texture* vel_txtr_green;

        // the center of mass of the craft
        SDL_Point rot_abt;

        // draw thrust, velocity, and fuel bars
        void draw_status(Screen& s);

        // dimensions of texture and spacecraft
        static const int WIDTH = 21;
        static const int HEIGHT = 40;
        static const int COLLISION_HEIGHT = 30;

        // collision points of triangle (corners)
        // in sc coords (rot_abt is center)
        // Looking at the SC with thrusters pointed down
        // 1 is bottom left      2
        // 2 is top             / \
        // 3 is bottom right   1 - 3
        // all units here are pixels
        // spacecraft coordinates
        const float sc_p1x = -WIDTH / 2.;
        const float sc_p1y = 0.;
        const float sc_p2x = 0.;
        const float sc_p2y = (float) COLLISION_HEIGHT;
        const float sc_p3x = WIDTH / 2.;
        const float sc_p3y = 0.;
        // world coordinates
        float p1x;
        float p1y;
        float p2x;
        float p2y;
        float p3x;
        float p3y;

        // Update position of corners in world coordinates.
        // Use sc_p#x and sc_p#y to compute p#x and p#y
        void update_corners();

        bool exploded = false; // hopefully we can keep it that way!

        bool is_safe_landing();
        bool is_colliding(const Ground& ground);
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
               float _dry_mass,
               float _init_fuel,
               float _max_thrust,
               float _exhaust_vel
        );
        ~Lander();

        void draw(Screen& s);
        void handle(SDL_Event* e);
        void move();
        void explode();
        World::CollisionResult check_collision(World& w);
        friend Pilot;
};

#endif
