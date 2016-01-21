#ifndef UTILS_H_
#define UTILS_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Screen.hpp"

namespace Utils {
    SDL_Texture* load_texture(SDL_Renderer* r,
                              const char filename[],
                              bool has_alpha); 
    SDL_Texture* create_text_texture(Screen& s,
                                     const char text[],
                                     SDL_Color* color);
    float rand_float(float lower_bd, float upper_bd);
    bool angle_diff(float a, float b, float* out);
    int round_nearest_int(float a);
}

#endif
