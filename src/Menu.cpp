#include <cstdlib>
#include "Menu.hpp"
#include "play.hpp"
#include "Utils.hpp"

Menu::MenuOption Menu::menu() {
    // TODO
    return MenuOption::QUICK_PLAY;
}

// How to play and press any key to begin.
// returning true means play and false means quit
bool Menu::how_to_play(Screen& s) {
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

void Menu::quick_play(Pilot* pilot, unsigned int seed) {
    // initialize, then call play() in a loop
    Screen s;
    bool again = true;
    if (pilot == NULL) {
        again = how_to_play(s);
    }
    PlayResult result(false, QUIT, 0);

    while (again) {
        // play the game
        result = play(&s, pilot, seed);

        // read their choice (Quit, New game, or replay)
        if (result.choice == QUIT) {
            again = false;
        } else if (result.choice == NEW_GAME) {
            seed = (unsigned int) time(NULL);
        }

        // recreate the Pilot if we need to
        if (pilot != NULL) {
            delete pilot;
            if (again) {
                pilot = new Pilot();
            }
        }
    }
}

