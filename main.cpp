// used this tutorial http://www.lazyfoo.net/tutorials/SDL/
// and copied some code from it.
#include <cstdio>
#include <sys/time.h>
#include <SDL2/SDL.h>
#include "Screen.hpp"
#include "Lander.hpp"
#include "constants.hpp"
#include "Ground.hpp"
#include "World.hpp"
#include "Pilot.hpp"

unsigned long play(bool human_player) {
    Screen s;
    World world(s, human_player);

    bool quit = false;
    SDL_Event e;
    unsigned long frame = 0;
    while (!quit) {
        struct timeval start = {0, 0};
        gettimeofday(&start, NULL);

        // Handle all events on queue
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            world.handle(&e);
        }
        world.move();

        World::CollisionResult r = world.check_collision();
        if (r == World::CollisionResult::WIN) {
            printf("You win\n");
            return frame;
        } else if (r == World::CollisionResult::LOSE) {
            printf("You lose\n");
            return frame;
        }

        s.clear();
        world.draw(s);
        s.flip();

        // sleep so that a frame takes FRAME_TIME
        struct timeval now = {0, 0};
        gettimeofday(&now, NULL);
        struct timeval diff = {0, 0};
        timersub(&now, &start, &diff);
        unsigned int sleep_time = FRAME_TIME - ((unsigned int) diff.tv_usec / 1000);
        if (FRAME_TIME > diff.tv_usec / 1000) {
            SDL_Delay(FRAME_TIME - diff.tv_usec / 1000);
        }
        frame++;
    }

    return frame;
}

int main(int argc, char** argv) {
    bool human_player = false;
    if (argc == 2 && strcmp(argv[1], "-c") == 0) {
        human_player = true;
    }
    unsigned long frames = play(human_player);
    //printf("elapsed %lu.%lu\n", frames * FRAME_TIME / 1000, frames * 100);
}

