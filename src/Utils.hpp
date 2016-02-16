#ifndef UTILS_H_
#define UTILS_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_keycode.h>
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
        // returns true if difference passes thru zero
        //
        // out is the smaller angle between a and b.
        //
        // a and b must be in the range [0 .. 2pi]
        // out is in the range [0 .. pi]
        static bool abs_angle_diff(double a, double b, double* out);

        // compute the difference between two angles (a - b)
        // returns true if difference passes thru zero
        //
        // out is the smaller angle between a and b
        // the sign of out satisfies this equation
        // a = (b + out) mod 2pi
        //
        // a and b must be in the range [0 .. 2pi]
        // out is in the range [-pi .. pi]
        static bool angle_diff(double a, double b, double* out);

        // round a double to the nearest integer
        // ties round up (1.5 -> 2)
        static int nearest_int(double a);
        
        // keys that windowing managers use, etc.
        static bool is_mod_key(SDL_Keycode k);
};

#endif
