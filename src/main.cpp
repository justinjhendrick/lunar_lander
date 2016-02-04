// used this tutorial http://www.lazyfoo.net/tutorials/SDL/
// and copied some code from it.
#include <cstdio>
#include <cstdlib>
#include "Screen.hpp"
#include "constants.hpp"
#include "Pilot.hpp"
#include "Utils.hpp"
#include "play.hpp"

/***************** TODO *******************************************************
 * Testing
 * Better design pattern compliance
 *     Like hiding internal details inside classes
 *     Split physics out of the lander
 *     no magic numbers
 * Competition
 *     Score for time and fuel used
 *     compete against computer and/or other humans
 * Make Lander::torque actually torque, not angular acceleration
 ******************************************************************************/

void invalid_args() {
    fprintf(stderr, "Invalid command line arguments\n.");
    fprintf(stderr, "Usage: ./lunar_lander [-c] [-s <seed>]\n");
}

int main(int argc, char** argv) {
    Pilot* p = NULL;
    unsigned int seed = (unsigned int) time(NULL);
    bool user_supplied_seed = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            p = new Pilot;
        } else if (strcmp(argv[i], "-s") == 0) {
            if (i + 1 < argc) {
                i++;
                char* endptr;
                seed = (unsigned int)strtoul(argv[i], &endptr, 10);
                if (endptr == argv[i]) {
                    // what followed -s is not a number
                    invalid_args();
                    return 1;
                }
                user_supplied_seed = true;
            } else {
                // nothing followed -s
                invalid_args();
                return 1;
            }
        }
    }

    Screen s;
    bool again = true;
    PlayResult result(false, QUIT);
    while (again) {

        result = play(&s, p, seed);
        if (result.choice == QUIT) {
            again = false;
        } else if (result.choice == NEW_GAME) {
            seed = (unsigned int) time(NULL);
        }

        if (p != NULL) {
            delete p;
            if (again) {
                p = new Pilot();
            }
        }
    }
}

