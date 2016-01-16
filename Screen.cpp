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
    if (TTF_Init() == -1) {
        fprintf(stderr, "no ttf: %s\n", TTF_GetError());
    }
    //font = TTF_OpenFont("1979_dot_matrix.ttf", 12);
    font = TTF_OpenFont("UbuntuMono-R.ttf", 12);
    if (font == NULL) {
        fprintf(stderr, "no font: %s\n", TTF_GetError());
    }
}

Screen::~Screen() {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
}

void Screen::flip() {
    SDL_RenderPresent(renderer);
}

void Screen::clear() {
    SDL_RenderClear(renderer);
}
