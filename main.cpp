// used this tutorial http://www.lazyfoo.net/tutorials/SDL/
// and copied some code from it.
#include <cstdio>
#include <cstdlib>
#include <random>
#include <sys/time.h>
#include <SDL2/SDL.h>
#include <time.h>
#include "Screen.hpp"
#include "Lander.hpp"
#include "constants.hpp"
#include "Ground.hpp"
#include "World.hpp"
#include "Pilot.hpp"
#include "Utils.hpp"

void end_game(Screen& s,
              World& world,
              Lander& lander,
              SDL_Texture* text,
              bool win) {
    SDL_Rect where;
    where.w = Screen::WIDTH / 4;
    where.h = Screen::HEIGHT / 4;
    where.x = Screen::WIDTH / 2 - where.w / 2;
    where.y = Screen::HEIGHT / 2 - where.h / 2;
    if (!win) {
        lander.explode();
    }

    s.clear();
    lander.draw(s);
    world.draw(s);
    SDL_RenderCopy(s.renderer, text, NULL, &where);
    s.flip();
    SDL_Delay(1000);
}

unsigned long play(Pilot* pilot) {
    Screen s;
    World world;
    Lander lander(s);
    SDL_Texture* win_text = Utils::create_text_texture(s, "You win!", NULL);
    SDL_Texture* lose_text = Utils::create_text_texture(s, "You lose", NULL);

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
            end_game(s, world, lander, win_text, true);
            return frame;
        } else if (r == World::CollisionResult::LOSE) {
            end_game(s, world, lander, lose_text, false);
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
        unsigned int sleep_time = FRAME_TIME -
            ((unsigned int) diff.tv_usec / 1000);
        if (FRAME_TIME > diff.tv_usec / 1000) {
            SDL_Delay(sleep_time);
        } else {
            printf("tired\n");
        }
        frame++;
    }

    return frame;
}

void invalid_args() {
    printf("Invalid command line arguments\n.");
    printf("Usage: ./lunar_lander [-c] [-s <seed>]\n");
}

int main(int argc, char** argv) {
    Pilot* p = NULL;
    unsigned int seed = (unsigned int) time(NULL);
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            p = new Pilot;
        } else if (strcmp(argv[i], "-s") == 0) {
            if (i + 1 < argc) {
                i++;
                char* endptr;
                seed = (unsigned int)strtoul(argv[i], &endptr, 10);
                if (endptr == argv[i]) {
                    invalid_args();
                    return 1;
                }
            } else {
                invalid_args();
                return 1;
            }
        }
    }
    printf("seed = %u\n", seed);
    Utils::init(seed);
    unsigned long frames = play(p);
    //printf("elapsed %lu.%lu\n", frames * FRAME_TIME / 1000, frames * 100);
    delete p;
    printf("seed = %u\n", seed);
}

