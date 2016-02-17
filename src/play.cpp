#include <SDL2/SDL.h>
#include <sys/time.h>
#include "Screen.hpp"
#include "Lander.hpp"
#include "constants.hpp"
#include "Ground.hpp"
#include "World.hpp"
#include "Pilot.hpp"
#include "Utils.hpp"
#include "play.hpp"

// Show player endgame screen and read their response
// The endgame screen prompts them to replay (r), start a
// new game (n) or quit (q)
EndGameOpt end_game(Screen& s,
              World& world,
              Lander& lander,
              bool win) {
    if (!win) {
        lander.explode();
    }

    // Take input to get response to "play again?"
    SDL_Event e;
    while (true) {
        s.clear();
        lander.draw(s);
        world.draw(s);
        s.put_endgame_text(win);
        s.flip();
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                return QUIT;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_n) {
                    return NEW_GAME;
                } else if (e.key.keysym.sym == SDLK_r) {
                    return REPLAY;
                } else if (e.key.keysym.sym == SDLK_q) {
                    return QUIT;
                }
            }
        }
        SDL_Delay(100);
    }
}

PlayResult::PlayResult(bool _win, EndGameOpt _choice) {
    win = _win;
    choice = _choice;
}

// play the game defined by the seed.
// A NULL pilot is a human player
// a NULL screen means headless mode. (used for testing)
PlayResult play(Screen* s, Pilot* pilot, unsigned int seed) {
    printf("seed = %u\n", seed);
    Utils::seed_random(seed);

    World world;
    Lander lander(s);

    SDL_Event e;
    while (true) {
        struct timeval start = {0, 0};
        gettimeofday(&start, NULL);

        // Handle all events on queue
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                return PlayResult(false, QUIT);
            }
            if (pilot == NULL) {
                lander.handle(&e);
            }
        }
        if (pilot != NULL) {
            pilot->fly(lander, world);
        }

        lander.move();

        World::CollisionResult r = world.check_collision(lander);
        if (r == World::CollisionResult::PAD) {
            bool safe = lander.is_safe_landing();
            if (safe) {
                printf("win\n");
                if (s != NULL) {
                    return PlayResult(true, end_game(*s, world, lander, true));
                } else {
                    return PlayResult(true, NEW_GAME);
                }
            } else {
                r = World::CollisionResult::LOSE;
            }
        } 
        
        if (r == World::CollisionResult::LOSE) {
            printf("lose\n");
            if (s != NULL) {
                return PlayResult(false, end_game(*s, world, lander, false));
            } else {
                return PlayResult(false, NEW_GAME);
            }
        }

        if (s != NULL) {
            s->clear();
            lander.draw(*s);
            world.draw(*s);
            s->flip();
        }

        // sleep so that a frame takes FRAME_TIME
        // This is important for the physics simulations.
        // If all frames take the same amout of time,
        // the physics is independent of the framerate.
        struct timeval now = {0, 0};
        gettimeofday(&now, NULL);
        struct timeval diff = {0, 0};
        timersub(&now, &start, &diff);
        unsigned int sleep_time = FRAME_TIME -
            ((unsigned int) diff.tv_usec / 1000);
        if (sleep_time > 0) {
            SDL_Delay(sleep_time);
        } else {
            printf("tired\n");
        }
    }
}
