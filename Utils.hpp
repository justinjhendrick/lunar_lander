#ifndef UTILS_H_
#define UTILS_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <random>
#include "Screen.hpp"

class Utils {
    private:
        static std::mt19937 rand_generator;
    public:
        static void init(unsigned int seed);
        static SDL_Texture* load_texture(SDL_Renderer* r,
                                  const char filename[]);
        static SDL_Texture* create_text_texture(Screen& s,
                                         const char text[],
                                         SDL_Color* color);
        static float rand_float(float lower_bd, float upper_bd);
        static int rand_int(int lower_bd, int upper_bd);
        static bool angle_diff(float a, float b, float* out);
        static int nearest_int(float a);
};

#endif
