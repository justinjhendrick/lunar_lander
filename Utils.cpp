#include <cstdlib>
#include "Utils.hpp"

SDL_Texture* Utils::load_texture(SDL_Renderer* r, const char filename[]) {
    SDL_Surface* surf = SDL_LoadBMP(filename);
    if (surf == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return NULL;
    }
    SDL_Texture* result = SDL_CreateTextureFromSurface(r, surf);
    if (result == NULL) {
        fprintf(stderr, "no convert to texture: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(surf);
    return result;
}

SDL_Texture* Utils::create_text_texture(Screen& s,
                                        const char text[],
                                        SDL_Color* color) {
    SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
    if (color == NULL) {
        color = &white;
    }
    SDL_Surface* surf = TTF_RenderText_Solid(s.font, text, *color);
    if (surf == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return NULL;
    }
    SDL_Texture* result = SDL_CreateTextureFromSurface(s.renderer, surf);
    if (result == NULL) {
        fprintf(stderr, "no convert to texture: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(surf);
    return result;
}

float Utils::rand_float(float lower_bd, float upper_bd) {
    return ((float)rand() / (float)(RAND_MAX)) * (upper_bd - lower_bd) + lower_bd;
}
