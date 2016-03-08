// used this tutorial http://www.lazyfoo.net/tutorials/SDL/
// and copied some code from it.
#include <SDL2/SDL.h>
#include <cstdio>
#include <cstdlib>
#include "Screen.hpp"
#include "constants.hpp"
#include "Pilot.hpp"
#include "Utils.hpp"
#include "Menu.hpp"


/***************** TODO *******************************************************
 * Make Pilot aware of Ground obstacles
 *     bool collide_p1 Physics::send(Particle lander.p1, World w);
 *     bool collide_p2 Physics::send(particle lander.p2, World w);
 *     bool collide_p3 Physics::send(particle lander.p3, World w);
 *     return collide_p1 || collide_p2 || collide_p3;
 * }
 *
 * Menu
 *     Quick play
 *     How to play
 *     versus mode -- compete against friends and/or the autopilot
 *     Watch auto pilot
 * Versus mode
 *     Score calculation screen
 *     colors for each player
 * More testing
 * Better design pattern compliance
 *     Like hiding internal details inside classes
 *     Split physics out of the lander?
 * Competition
 *     compete against computer and/or other humans
 * Make Lander::torque actually torque, not angular acceleration
 *     compute STOP_ABOVE_PAD based on rotation time
 ******************************************************************************/

void invalid_args() {
    fprintf(stderr, "Invalid command line arguments\n.");
    fprintf(stderr, "Usage: ./lunar_lander [-c] [-s <seed>]\n");
}

int main(int argc, char** argv) {

    // read command line arguments
    Pilot* pilot = NULL;
    unsigned int seed = (unsigned int) time(NULL);
    bool any_args = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            any_args = true;
            // -c supplied, create computer controlled pilot
            pilot = new Pilot;
        } else if (strcmp(argv[i], "-s") == 0) {
            any_args = true;
            // a number should follow -s.
            // Use it as a seed to random number generator
            if (i + 1 < argc) {
                i++;
                char* endptr;
                seed = (unsigned int)strtoul(argv[i], &endptr, 10);
                if (endptr == argv[i]) {
                    // what followed -s is not a number
                    invalid_args();
                    return 1;
                }
            } else {
                // nothing followed -s
                invalid_args();
                return 1;
            }
        }
    }
    if (any_args) {
        Menu::quick_play(pilot, seed);
    }
    Menu::MenuOption choice = Menu::menu();
    if (choice == Menu::QUICK_PLAY) {
        Menu::quick_play(pilot, seed);
    }
}
