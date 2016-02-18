#include <random>
#include "Utils.hpp"

std::mt19937 Utils::rand_generator;

void Utils::seed_random(unsigned int seed) {
    rand_generator.seed(seed);
}


double Utils::rand_double(double lower_bd, double upper_bd) {
    std::uniform_real_distribution<double> dist(lower_bd, upper_bd);
    return dist(rand_generator);
}

int Utils::rand_int(int lower_bd, int upper_bd) {
    std::uniform_int_distribution<int> dist(lower_bd, upper_bd);
    return dist(rand_generator);
}


bool Utils::abs_angle_diff(double a, double b, double* out) {
    double result;
    bool thru_zero = angle_diff(a, b, &result);
    *out = fabs(result);
    return thru_zero;
}

bool Utils::angle_diff(double a, double b, double* out) {
    double diff = a - b;
    bool thru_zero = false;
    // shouldn't ever rotate more than half circle
    if (diff > M_PI) {
        diff = -2 * M_PI + diff;
        thru_zero = true;
    } else if (diff < -M_PI) {
        diff = 2 * M_PI + diff;
        thru_zero = true;
    }
    *out = diff;
    return thru_zero;
}

int Utils::nearest_int(double a) {
    return (int) floor(a + 0.5);
}

// https://wiki.libsdl.org/SDLKeycodeLookup
bool Utils::is_mod_key(SDL_Keycode k) {
    switch (k) {
        case SDLK_LALT:
        case SDLK_RALT:
        case SDLK_LSHIFT:
        case SDLK_RSHIFT:
        case SDLK_CAPSLOCK:
        case SDLK_LCTRL:
        case SDLK_RCTRL:
        case SDLK_TAB:
        case SDLK_LGUI: // aka super, meta, windows, command (apple)
        case SDLK_RGUI:
            return true;
        default:
            return false;
    }
}

// return an angle in radians (0 .. 2pi)
// that represents an orientation where
// thrusters point in the direction of travel
double Utils::compute_retrograde(double x_vel, double y_vel) {
    double retrograde = atan2(y_vel, x_vel);
    retrograde += M_PI;
    if (retrograde >= 2 * M_PI) {
        retrograde -= 2 * M_PI;
    }
    return retrograde;
}

std::pair<double, double> Utils::rotate(double x, double y,
                                        double angle) {
    // apply a rotation matrix
    // https://en.wikipedia.org/wiki/Rotation_matrix
    std::pair<double, double> result;
    double s = sin(angle);
    double c = cos(angle);
    result.first  = c * x - s * y;
    result.second = s * x + c * y;
    return result;
}
