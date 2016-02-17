#ifndef LANDER_H_
#define LANDER_H_

#include <stdint.h>
#include "Screen.hpp"
#include "Ground.hpp"
#include "World.hpp"

// forward declare Pilot so it can be a friend
class Pilot;

class Lander : public Physics {
    private:
        // initialized by constructor
        double orientation; // radians cw from x axis
        double spin_rate;   // radians/second. positive is cw, negative is ccw
                            // Torque is misnomer.
                            // it's actually angular acceleration.
        double fuel;        // kg
        double dry_mass;    // kg (const)
        double init_fuel;   // kg (const)
        double thrust;      // Newtons
        double max_thrust;  // Newtons (const)
        double exhaust_vel; // m/s

        bool thrusting = false;
        double torque = 0.;      // radians/s^2.  torque is misnomer.
        constexpr static const double SAFE_VEL = 15.; // pixels/s
        constexpr static const double MAX_VEL = 100.; // pixels/s. only cosmetic
        // radians from 3 * PI / 2
        constexpr static const double SAFE_ORIENTATION = M_PI / 8; 
        constexpr static const double MAX_TORQUE = 5.;  // radians/s^2

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

        Line lines[3] = {Line(0, 0, 0, 0),
                         Line(0, 0, 0, 0),
                         Line(0, 0, 0, 0)};

        // Update position of corners in world coordinates.
        // Use sc_p#x and sc_p#y to compute p#x and p#y
        //
        // then, update the lines from the corner info
        void update_lines();

        bool exploded = false; // hopefully we can keep it that way!

        bool is_colliding(const Ground& ground);

    public:
        Lander(Screen* s);
        Lander(Screen* s,
               double _x_pos,
               double _y_pos,
               double _x_vel,
               double _y_vel,
               double _orientation,
               double _spin_rate,
               double _dry_mass,
               double _init_fuel,
               double _max_thrust,
               double _exhaust_vel
        );
        ~Lander();

        void draw(Screen& s);

        bool is_safe_landing();

        // React to keys that control the Lander
        void handle(SDL_Event* e);

        // Under the current Lander conditions,
        // what would the velocity in the next frame be?
        std::pair<double, double> next_velocity();

        // implementing virtuals from Physics
        void perturb() override;
        Physics::VelAccel next_vel_accel(bool real) override;
        void get_lines(std::vector<Line>& lines_out) override;

        // for changing the texture to an explosion
        void explode();

        friend Pilot;
};

#endif
