#include <cstdlib>
#include "Utils.hpp"

SDL_Texture* Utils::load_texture(
        SDL_Renderer* r,
        const char filename[],
        bool has_alpha) {
    SDL_Surface* surf = SDL_LoadBMP(filename);
    if (surf == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return NULL;
    }
    //if (has_alpha) {
    //    printf("here\n");
    //    surf->format->Amask = 0xFF000000; 
    //    surf->format->Rmask = 0x00FF0000; 
    //    surf->format->Gmask = 0x0000FF00; 
    //    surf->format->Bmask = 0x000000FF; 
    //}
    SDL_Texture* result = SDL_CreateTextureFromSurface(r, surf);
    if (result == NULL) {
        fprintf(stderr, "no convert to texture: %s\n", SDL_GetError());
    }
    SDL_SetTextureBlendMode(result, SDL_BLENDMODE_BLEND);
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

// puts difference in 'out'
// returns true if it'll pass through zero
bool Utils::angle_diff(float a, float b, float* out) {
    float diff = fabs(a - b);
    bool result = false;
    // shouldn't ever rotate more than half circle
    if (diff > M_PI) {
        diff = 2 * M_PI - diff;
        result = true;
    }
    *out = diff;
    return result;
}

int Utils::round_nearest_int(float a) {
    return (int) floor(a + 0.5);
}
