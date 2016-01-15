// used this tutorial http://www.lazyfoo.net/tutorials/SDL/
// and copied some code from it.
#include <cstdio>
#include <ctime>
#include <SDL2/SDL.h>
#include "Screen.hpp"
#include "Lander.hpp"
#include "constants.hpp"
#include "Ground.hpp"

int main() {
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
    while (!quit) {
        time_t start = time(NULL);
        // Handle all events on queue
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            l.handle(&e);
        }
        l.move();

        if (pad.is_colliding(l)) {
            printf("You win\n");
            return 0;
        } else if (top.is_colliding(l) || left.is_colliding(l) ||
                   right.is_colliding(l) || bot.is_colliding(l)) {
            printf("You lose\n");
            return 0;
        }

        s.clear();
        l.draw(s);
        pad.draw(s);
        s.flip();

        time_t now = time(NULL);
        SDL_Delay(FRAME_TIME - (now - start));
    }

    return 0;
}
