#ifndef WORLD_H_
#define WORLD_H_

#include <vector>
#include <SDL2/SDL.h>
#include "Screen.hpp"
#include "Ground.hpp"

class World {
    public:
        enum CollisionResult {
            NO_COLLISION,
            WIN,
            LOSE
        };
        // list of all collidable surfaces. Starting with pad(s)
        std::vector<Ground> grounds;

        World();
        
        CollisionResult check_collision();
        void draw(Screen& s);
};

#endif
