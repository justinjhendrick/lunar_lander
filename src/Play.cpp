#include <SDL2/SDL.h>
#include <sys/time.h>
#include <cmath>
#include "Screen.hpp"
#include "Lander.hpp"
#include "constants.hpp"
#include "Ground.hpp"
#include "World.hpp"
#include "Pilot.hpp"
#include "Utils.hpp"
#include "Play.hpp"
#include "Player.hpp"

// Show player endgame screen and read their response
// The endgame screen prompts them to replay (r), start a
// new game (n) or quit (q)
Play::EndGameOpt Play::end_game(Screen& s,
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
                    return RETURN_TO_MENU;
                }
            }
        }
        SDL_Delay(100);
    }
}

Play::PlayResult::PlayResult(bool _win, EndGameOpt _choice, unsigned int _score) {
    win = _win;
    choice = _choice;
    score = _score;
}

// x must be > 0
// scale * 2 ^ (-x / stretch)
unsigned int Play::exponential_decay(double scale, double stretch, double x) {
    return (unsigned int) Utils::nearest_int(scale * pow(2., - x / stretch));
}

unsigned int Play::compute_score(bool win,
                                 unsigned long frames,
                                 double fuel_percent) {
    if (!win) {
        // You get nothing! You lose! Good day, sir!
        return 0;
    }
    
    const unsigned int WIN_BONUS = 100;
    const double TIME_SCALE = 248.5;
    const double TIME_STRETCH = 3.02;
    // The two above values were chosen by experimentation
    // I played a good game and a bad game and decided how many
    // points I deserved for each. Then solved for an exponential function
    // that intersects both of those points.
    const double MAX_FUEL_SCORE = 70.;

    double seconds_elapsed = frames * DT;
    unsigned int time_bonus =
        exponential_decay(TIME_SCALE, TIME_STRETCH, (double) seconds_elapsed);

    unsigned int fuel_bonus =
        (unsigned int) Utils::nearest_int(fuel_percent * MAX_FUEL_SCORE);

    return WIN_BONUS + time_bonus + fuel_bonus;
}

// play the game defined by the seed.
// A NULL pilot is a human player
// a NULL screen means headless mode. (used for testing)
Play::PlayResult Play::play(Screen* s, Pilot* pilot, unsigned int seed) {
    printf("seed = %u\n", seed);
    Utils::seed_random(seed);

    World world;
    Lander lander(s, Colors::PlayerColor::WHITE);

    SDL_Event e;
    unsigned long frames = 0;
    while (true) {
        struct timeval start = {0, 0};

        if (s != NULL) {
            gettimeofday(&start, NULL);
            // Handle all events on queue
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    return PlayResult(false, QUIT, 0.);
                }
                if (pilot == NULL) {
                    lander.handle(&e);
                }
            }
        }
        if (pilot != NULL) {
            pilot->fly(lander, world);
        }

        lander.move();

        World::CollisionResult r = world.check_collision(lander);
        if (r != World::CollisionResult::NO_COLLISION) {
            bool win = r == World::CollisionResult::PAD &&
                       lander.is_safe_landing();
            if (win) {
                printf("win\n");
            } else {
                printf("lose\n");
            }
            unsigned int score = compute_score(win,
                                               frames,
                                               lander.fuel_remaining());
            printf("score = %u\n", score);
            EndGameOpt decision;
            if (s != NULL) {
                decision = end_game(*s, world, lander, win);
            } else {
                decision = NEW_GAME;
            } 
            return PlayResult(win, decision, score);
        }
        
        if (s != NULL) {
            s->clear();
            lander.draw(*s);
            world.draw(*s);
            s->flip();

            // sleep so that a frame takes FRAME_TIME
            // This is important for the physics simulations.
            // If all frames take the same amout of time,
            // the physics is independent of the framerate.
            struct timeval now = {0, 0};
            gettimeofday(&now, NULL);
            struct timeval diff = {0, 0};
            timersub(&now, &start, &diff);
            long sleep_time = ((long) FRAME_TIME) -
                              ((long) diff.tv_usec / 1000);
            if (sleep_time > 0) {
                SDL_Delay((unsigned int) sleep_time);
            } else {
                printf("tired\n");
            }
        }
        frames++;
    }
}
