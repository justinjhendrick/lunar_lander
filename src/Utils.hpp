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
        // Initialize the random number generator
        // used by rand_float and rand_int
        static void init(unsigned int seed);
        
        // load a BMP
        static SDL_Texture* load_texture(SDL_Renderer* r,
                                  const char filename[]);

        // create a Texture with text in it
        static SDL_Texture* create_text_texture(Screen& s,
                                         const char text[],
                                         SDL_Color* color);

        // generate a pseudo-random number uniformly from the
        // range [lower_bd, upper_bd], including the endpoints
        static float rand_float(float lower_bd, float upper_bd);
        static int rand_int(int lower_bd, int upper_bd);

        // compute the absolute value of the difference between two angles
        static bool angle_diff(float a, float b, float* out);

        // round a float to the nearest integer
        static int nearest_int(float a);
};

#endif
