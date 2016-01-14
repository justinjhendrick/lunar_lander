// used this tutorial http://www.lazyfoo.net/tutorials/SDL/
// and copied some code from it.
#include <cstdio>
#include <SDL2/SDL.h>
#include "Screen.hpp"
#include "Lander.hpp"

int main(int argc, char** argv) {
    Screen s;
    Lander l;

    s.clear();
    l.draw(s);
    s.flip();

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        // Handle all events on queue
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    return 0;
}
