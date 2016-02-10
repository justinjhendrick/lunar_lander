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
        double x_pos;       // pixels. positive is to the right on the screen.
        double y_pos;       // positive is down the screen.
        double x_vel;       // meters/second
        double y_vel;
        double orientation; // radians cw from x axis
        double spin_rate;   // radians/second. positive is cw, negative is ccw
        double max_torque;  // radians/second^2 (const).
                           // Torque is misnomer.
                           // it's actually angular acceleration.
        double fuel;        // kg
        double dry_mass;    // kg (const)
        double init_fuel;   // kg (const)
        double thrust;      // Newtons
        double max_thrust;  // Newtons (const)
        double exhaust_vel; // m/s

        bool thrusting = false;
        double torque;                            // radians/s^2.
                                                 // torque is misnomer.
        double dt;                                // seconds
        double vel;                               // magnitude in pixels/s
        const double safe_vel = 15.;              // pixels/s
        const double max_vel = 100.;              // pixels/s. only cosmetic
        const double safe_orientation = M_PI / 8; // radians from 3 * PI / 2
        const int pixels_per_meter = 10;         // pixels/meter

        // image textures
        SDL_Texture* txtr = NULL;
        SDL_Texture* txtr_fire_low = NULL;
        SDL_Texture* txtr_fire_med = NULL;
        SDL_Texture* txtr_fire_high = NULL;
        SDL_Texture* txtr_explosion = NULL;
        SDL_Texture* txtr_torque_cw = NULL;
        SDL_Texture* txtr_torque_ccw = NULL;
        // text textures
        SDL_Texture* fuel_txtr = NULL;
        SDL_Texture* thrust_txtr = NULL;
        SDL_Texture* vel_txtr = NULL;
        SDL_Texture* vel_txtr_green = NULL;

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
        const double sc_p1x = -WIDTH / 2.;
        const double sc_p1y = 0.;
        const double sc_p2x = 0.;
        const double sc_p2y = (double) COLLISION_HEIGHT;
        const double sc_p3x = WIDTH / 2.;
        const double sc_p3y = 0.;
        // world coordinates
        double p1x;
        double p1y;
        double p2x;
        double p2y;
        double p3x;
        double p3y;

        // Update position of corners in world coordinates.
        // Use sc_p#x and sc_p#y to compute p#x and p#y
        void update_corners();

        bool exploded = false; // hopefully we can keep it that way!

        bool is_safe_landing();
        bool is_colliding(const Ground& ground);

        typedef struct VelAccel {
            double x_vel;
            double y_vel;
            double x_accel;
            double y_accel;
        } VelAccel;

        // computes next velocity (m/s) and and acceleration (m/s^2)
        // if [real] is true, fuel will be used
        VelAccel next_vel_accel(bool real);
    public:
        Lander(Screen* s);
        Lander(Screen* s,
               double _x_pos,
               double _y_pos,
               double _x_vel,
               double _y_vel,
               double _orientation,
               double _spin_rate,
               double _max_torque,
               double _dry_mass,
               double _init_fuel,
               double _max_thrust,
               double _exhaust_vel
        );
        ~Lander();

        void draw(Screen& s);

        // Look for keys that control the Lander
        void handle(SDL_Event* e);

        // Under the current Lander conditions,
        // what would the velocity in the next frame be?
        std::pair<double, double> next_velocity();

        // move the Lander
        void move();

        // for changing the texture to an explosion
        void explode();

        // Are we colliding with the ground or the pad?
        World::CollisionResult check_collision(World& w);

        friend Pilot;
};

#endif
