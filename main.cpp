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

unsigned long play(Pilot* pilot) {
    Screen s;
    World world;
    Lander lander(s);

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
            if (pilot == NULL) {
                lander.handle(&e);
            }
        }
        if (pilot != NULL) {
            pilot->fly(lander, world);
        }

        lander.move();

        World::CollisionResult r = lander.check_collision(world);
        if (r == World::CollisionResult::WIN) {
            printf("You win\n");
            return frame;
        } else if (r == World::CollisionResult::LOSE) {
            printf("You lose\n");
            return frame;
        }

        s.clear();
        lander.draw(s);
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
    Pilot* p = NULL;
    if (argc == 2 && strcmp(argv[1], "-c") == 0) {
        p = new Pilot;
    }
    unsigned long frames = play(p);
    //printf("elapsed %lu.%lu\n", frames * FRAME_TIME / 1000, frames * 100);
    delete p;
}

