#include <random>
#include "Utils.hpp"

std::mt19937 Utils::rand_generator;

void Utils::seed_random(unsigned int seed) {
    rand_generator.seed(seed);
}


float Utils::rand_float(float lower_bd, float upper_bd) {
    std::uniform_real_distribution<float> dist(lower_bd, upper_bd);
    return dist(rand_generator);
}

int Utils::rand_int(int lower_bd, int upper_bd) {
    std::uniform_int_distribution<int> dist(lower_bd, upper_bd);
    return dist(rand_generator);
}

// puts difference in 'out'
// returns true if it'll pass through zero
bool Utils::angle_diff(float a, float b, float* out) {
    float diff = fabs(a - b);
    bool thru_zero = false;
    // shouldn't ever rotate more than half circle
    if (diff > M_PI) {
        diff = 2 * M_PI - diff;
        thru_zero = true;
    }
    *out = diff;
    return thru_zero;
}

int Utils::nearest_int(float a) {
    return (int) floor(a + 0.5);
}
