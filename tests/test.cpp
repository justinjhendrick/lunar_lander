#include <stdlib.h>

extern void test_utils();
extern void test_vector();
extern void test_pilot(unsigned int n_trials);

#define DEFAULT_TEST_TRIALS 1

// usage:
// ./test.exe [<n>]
//
// where n is a integer representing how many times to test the
// autopilot. Default value is 1
int main(int argc, char** argv) {
    // read args
    long num_pilot_trials = DEFAULT_TEST_TRIALS;
    if (argc == 2) {
        char* end_ptr = NULL;
        long tmp = strtol(argv[1], &end_ptr, 10);
        if (end_ptr != argv[1]) {
            num_pilot_trials = tmp;
        }
    }

    // run tests
    test_utils();
    test_vector();
    test_pilot(num_pilot_trials);
    return 0;
}
