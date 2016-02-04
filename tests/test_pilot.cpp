#include <cstdlib>
#include <time.h>
#include "../src/play.hpp"

void test_pilot(unsigned int n_trials) {
    for (int i = 0; i < n_trials; i++) {
        Pilot p;
        unsigned int seed = (unsigned int) time(NULL);
        PlayResult result = play(NULL, &p, seed);
        if (!result.win) {
            printf("bad seed %u", seed);
        }
    }
}
