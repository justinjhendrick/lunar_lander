#ifndef WORLD_H_
#define WORLD_H_

#include <vector>
#include <SDL2/SDL.h>
#include "Screen.hpp"
#include "Ground.hpp"

class World {
    private:
        void generate_terrain(int init_x, int init_y, int dx);
    public:
        enum CollisionResult {
            NO_COLLISION,
            WIN,
            LOSE
        };
        // list of all collidable surfaces. Starting with pad(s)
        std::vector<Ground> grounds;
        // gravitational acceleration, meters/s^2
        constexpr static const float g = 1.62;

        World();

        void draw(Screen& s);
        Ground& get_pad();
};

#endif
