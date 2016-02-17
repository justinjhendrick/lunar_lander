// used this tutorial http://www.lazyfoo.net/tutorials/SDL/
// and copied some code from it.
#include <SDL2/SDL.h>
#include <cstdio>
#include <cstdlib>
#include "Screen.hpp"
#include "constants.hpp"
#include "Pilot.hpp"
#include "Utils.hpp"
#include "play.hpp"


/***************** TODO *******************************************************
 * Make Pilot aware of Ground obstacles
 *     bool collide_p1 Physics::send(Particle lander.p1, World w);
 *     bool collide_p2 Physics::send(particle lander.p2, World w);
 *     bool collide_p3 Physics::send(particle lander.p3, World w);
 *     return collide_p1 || collide_p2 || collide_p3;
 * }
 *     
 * More testing
 * Better design pattern compliance
 *     Like hiding internal details inside classes
 *     Split physics out of the lander?
 * Competition
 *     Score for time and fuel used
 *     compete against computer and/or other humans
 * Make Lander::torque actually torque, not angular acceleration
 ******************************************************************************/

void invalid_args() {
    fprintf(stderr, "Invalid command line arguments\n.");
    fprintf(stderr, "Usage: ./lunar_lander [-c] [-s <seed>]\n");
}

// Main menu. Title, How to play, and press any key to begin.
// returning true means play and false means quit
bool menu(Screen& s) {
    // set up rectangles for text
    SDL_Rect title;
    title.w = Screen::WIDTH / 4;
    title.h = 3 * Screen::HEIGHT / 16;
    title.x = Screen::WIDTH / 2 - title.w / 2;
    title.y = Screen::HEIGHT / 16;
    
    SDL_Rect where;
    where.w = 3 * Screen::WIDTH / 4;
    where.h = Screen::HEIGHT / 16;
    where.x = Screen::WIDTH / 2 - where.w / 2;

    // create text textures
    SDL_Texture* title_text[7];
    title_text[0] = s.create_text_texture( "Lunar Lander", NULL);
    title_text[1] = s.create_text_texture(
            "left  or a     apply torque counterclockwise", NULL);
    title_text[2] = s.create_text_texture(
            "right or d     apply torque clockwise       ", NULL);
    title_text[3] = s.create_text_texture(
            "Spacebar       fire thruster                ", NULL);
    title_text[4] = s.create_text_texture(
            "up    or w     increase thrust              ", NULL);
    title_text[5] = s.create_text_texture(
            "down  or s     decrease thrust              ", NULL);
    title_text[6] = s.create_text_texture(
            "            Press any key to start          ", NULL);

    SDL_Event e;
    while (true) {
        // write text to the screen
        s.clear();
        where.y = Screen::HEIGHT / 6 - where.h / 2;
        for (int i = 0; i < 7; i++) {
            SDL_Rect rect = (i == 0 ? title : where);
            SDL_RenderCopy(s.renderer, title_text[i], NULL, &rect);
            where.y += rect.h;
            if (i == 5) {
                where.y += rect.h;
            }
        }
        s.flip();

        // Read input
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                return false;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    return false;
                } else if (!Utils::is_mod_key(e.key.keysym.sym)) {
                    return true;
                }
            }
        }
        SDL_Delay(100);
    }
}

int main(int argc, char** argv) {

    // read command line arguments
    Pilot* p = NULL;
    unsigned int seed = (unsigned int) time(NULL);
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            // -c supplied, create computer controlled pilot
            p = new Pilot;
        } else if (strcmp(argv[i], "-s") == 0) {
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

    // initialize, then call play() in a loop
    Screen s;
    bool again = true;
    if (p == NULL) {
        again = menu(s);
    }
    PlayResult result(false, QUIT);
    while (again) {

        // play the game
        result = play(&s, p, seed);

        // read their choice (Quit, New game, or replay)
        if (result.choice == QUIT) {
            again = false;
        } else if (result.choice == NEW_GAME) {
            seed = (unsigned int) time(NULL);
        }

        // recreate the Pilot if we need to
        if (p != NULL) {
            delete p;
            if (again) {
                p = new Pilot();
            }
        }
    }
}

