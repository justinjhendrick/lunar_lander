#include "Screen.hpp"

Screen::Screen() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL no init: %s\n", SDL_GetError());
    }
    if (SDL_CreateWindowAndRenderer(WIDTH,
                                    HEIGHT,
                                    SDL_WINDOW_SHOWN,
                                    &window,
                                    &renderer) < 0) {
        fprintf(stderr, "No window & renderer: %s\n", SDL_GetError());
    }
}

Screen::~Screen() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Screen::flip() {
    SDL_RenderPresent(renderer);
}

void Screen::clear() {
    SDL_RenderClear(renderer);
}
