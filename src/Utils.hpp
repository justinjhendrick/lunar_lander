#ifndef UTILS_H_
#define UTILS_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <random>

class Utils {
    private:
        static std::mt19937 rand_generator;
    public:
        // Initialize the random number generator
        // used by rand_double and rand_int
        static void seed_random(unsigned int seed);

        // generate a pseudo-random number uniformly from the
        // range [lower_bd, upper_bd], including the endpoints
        static double rand_double(double lower_bd, double upper_bd);
        static int rand_int(int lower_bd, int upper_bd);

        // compute the absolute value of the difference between two angles
        static bool angle_diff(double a, double b, double* out);

        // round a double to the nearest integer
        // ties round up (1.5 -> 2)
        static int nearest_int(double a);
};

#endif
