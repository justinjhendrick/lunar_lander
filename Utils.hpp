#ifndef UTILS_H_
#define UTILS_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Screen.hpp"

namespace Utils {
    SDL_Texture* load_texture(SDL_Renderer* r, const char filename[]);
    SDL_Texture* create_text_texture(Screen& s, const char text[]);
}

#endif
