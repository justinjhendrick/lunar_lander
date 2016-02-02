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

/***************** TODO *******************************************************
 * Testing
 * Better design pattern compliance
 *     Like hiding internal details inside classes
 *     Split physics out of the lander
 *     no magic numbers
 * Competition
 *     Score for time and fuel used
 *     compete against computer and/or other humans
 * Make Lander::torque actually torque, not angular acceleration
 ******************************************************************************/

// returns true if the player wants to play again
bool end_game(Screen& s,
              World& world,
              Lander& lander,
              SDL_Texture* text,
              bool win) {
    SDL_Rect where;
    where.w = 3 * Screen::WIDTH / 4;
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

    // Take input to get response to "play again?"
    SDL_Event e;
    while (true) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                return false;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_n) {
                    return false;
                } else if (e.key.keysym.sym == SDLK_y) {
                    return true;
                }
            }
        }
        SDL_Delay(100);
    }
}

// play the game. A NULL pilot is a human player
// returns true if the player wants to play again.
bool play(Screen& s, Pilot* pilot) {
    World world;
    Lander lander(s);
    SDL_Texture* win_text = s.create_text_texture(
            "You win! Play again? y/n", NULL);
    SDL_Texture* lose_text = s.create_text_texture(
            "You lose. Play again? y/n", NULL);

    SDL_Event e;
    unsigned long frame = 0;
    while (true) {
        struct timeval start = {0, 0};
        gettimeofday(&start, NULL);

        // Handle all events on queue
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                return false;
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
            printf("win\n");
            return end_game(s, world, lander, win_text, true);
        } else if (r == World::CollisionResult::LOSE) {
            printf("lose\n");
            return end_game(s, world, lander, lose_text, false);
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
    }
}

void invalid_args() {
    fprintf(stderr, "Invalid command line arguments\n.");
    fprintf(stderr, "Usage: ./lunar_lander [-c] [-s <seed>]\n");
}

int main(int argc, char** argv) {
    Pilot* p = NULL;
    unsigned int seed = (unsigned int) time(NULL);
    bool user_supplied_seed = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            p = new Pilot;
        } else if (strcmp(argv[i], "-s") == 0) {
            if (i + 1 < argc) {
                i++;
                char* endptr;
                seed = (unsigned int)strtoul(argv[i], &endptr, 10);
                if (endptr == argv[i]) {
                    // what followed -s is not a number
                    invalid_args();
                    return 1;
                }
                user_supplied_seed = true;
            } else {
                // nothing followed -s
                invalid_args();
                return 1;
            }
        }
    }

    // new map every time they replay without -s <seed>
    if (!user_supplied_seed) {
        Utils::seed_random(time(NULL));
    }

    Screen s;
    bool again = true;
    while (again) {
        // same map every time with -s <seed>
        if (user_supplied_seed) {
            Utils::seed_random(seed);
        }
        again = play(s, p);
        if (p != NULL) {
            delete p;
            p = new Pilot();
        }
    }
    printf("seed = %u\n", seed);
}

