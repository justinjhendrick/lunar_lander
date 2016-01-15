// used this tutorial http://www.lazyfoo.net/tutorials/SDL/
// and copied some code from it.
#include <cstdio>
#include <ctime>
#include <SDL2/SDL.h>
#include "Screen.hpp"
#include "Lander.hpp"
#include "constants.hpp"

int main(int argc, char** argv) {
    Screen s;
    Lander l(s);

    s.clear();
    l.draw(s);
    s.flip();

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

        s.clear();
        l.draw(s);
        s.flip();

        time_t now = time(NULL);
        SDL_Delay(FRAME_TIME - (now - start));
    }

    return 0;
}
