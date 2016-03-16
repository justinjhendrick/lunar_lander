#include <cstdlib>
#include <time.h>
#include <assert.h>
#include "../src/play.hpp"

void test_pilot(unsigned int n_trials) {
    unsigned int seed = (unsigned int) time(NULL);
    bool all_good = true;
    for (int i = 0; i < n_trials; i++) {
        Pilot p;
        PlayResult result = play(NULL, &p, seed);
        if (!result.win) {
            all_good = false;
            printf("########## LOSS! ######### BAD SEED = %u", seed);
        }
        seed = rand();
    }
    assert(all_good);
}
