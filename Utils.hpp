#ifndef UTILS_H_
#define UTILS_H_

#include <SDL2/SDL.h>

namespace Utils {
    SDL_Texture* load_texture(SDL_Renderer* r, const char filename[]);
}

#endif
