// used this tutorial http://www.lazyfoo.net/tutorials/SDL/
// and copied some code from it.
#include <cstdio>
#include <sys/time.h>
#include <SDL2/SDL.h>
#include "Screen.hpp"
#include "Lander.hpp"
#include "constants.hpp"
#include "Ground.hpp"

int play(bool);

int main(int argc, char** argv) {
    bool human_player = true;
    if (argc == 2 && strcmp(argv[1], "-c") == 0) {
        human_player = false;
    }
    unsigned long frames = play(human_player);
    printf("elapsed %lu.%lu\n", frames * FRAME_TIME / 1000, frames * 100);
}

int play(bool human_player) {
    Screen s;
    Lander l(s);
    Ground pad(true, Screen::WIDTH / 2 - 20, Screen::HEIGHT - 10,
                     Screen::WIDTH / 2 + 20, Screen::HEIGHT - 10);
    Ground top(false, 0, 0, Screen::WIDTH, 0);
    Ground left(false, 0, 0, 0, Screen::HEIGHT);
    Ground right(false, Screen::WIDTH, 0, Screen::WIDTH, Screen::HEIGHT);
    Ground bot(false, 0, Screen::HEIGHT, Screen::WIDTH, Screen::HEIGHT);

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
            if (human_player) {
                l.handle(&e);
            }
        }
        if (!human_player) {
            l.fly_self(pad);
        }
        l.move();

        if (l.is_colliding(pad)) {
            if (l.safe_landing()) {
                printf("You win\n");
            } else {
                printf("You lose\n");
            }
            return frame;
        } else if (l.is_colliding(top) || l.is_colliding(left) ||
                   l.is_colliding(right) || l.is_colliding(bot)) {
            printf("You lose\n");
            return frame;
        }

        s.clear();
        l.draw(s);
        pad.draw(s);
        s.flip();

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
