#ifndef WORLD_H_
#define WORLD_H_

#include <vector>
#include <SDL2/SDL.h>
#include "Screen.hpp"
#include "Ground.hpp"
#include "Physics.hpp"

// a class to represent the world that the Lander flies in
class World {
    private:
        // randomly generate terrain
        void generate_terrain(int init_x, int init_y, int dx);
    public:
        enum CollisionResult {
            NO_COLLISION,
            PAD,
            LOSE
        };
        // list of all collidable surfaces. Starting with pad(s)
        std::vector<Ground> grounds;
        // gravitational acceleration, meters/s^2
        constexpr static const double g = 1.62;
        static const int SEGMENT_WIDTH = 40; // pixels
        static const int MAX_DY = 120; // pixels

        World();

        void draw(Screen& s);
        Ground& get_pad();

        // Are we colliding with the ground, the pad, or nothing?
        CollisionResult check_collision(Physics& p);
};

#endif
