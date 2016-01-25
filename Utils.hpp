#ifndef UTILS_H_
#define UTILS_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <random>
#include "Screen.hpp"

class Utils {
    private:
        static std::default_random_engine rand_generator;
    public:
        static void init(unsigned int seed);
        static SDL_Texture* load_texture(SDL_Renderer* r,
                                  const char filename[],
                                  bool has_alpha); 
        static SDL_Texture* create_text_texture(Screen& s,
                                         const char text[],
                                         SDL_Color* color);
        static float rand_float(float lower_bd, float upper_bd);
        static float rand_int(int lower_bd, int upper_bd);
        static bool angle_diff(float a, float b, float* out);
        static int round_nearest_int(float a);
};

#endif
