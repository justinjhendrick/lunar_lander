#include <cstdlib>
#include "Menu.hpp"
#include "Play.hpp"
#include "Utils.hpp"

Menu::MenuOption Menu::menu(Screen& s) {

    SDL_Rect logo_box;
    SDL_Texture* logo = s.load_texture("imgs/logo.bmp");

    // get logo_box width and height from texture
    Uint32 unused_format;
    int unused_access;
    if (SDL_QueryTexture(logo,
                         &unused_format,
                         &unused_access,
                         &logo_box.w,
                         &logo_box.h) != 0) {
        printf("test\n");
        fprintf(stderr, "cannot read width and height from texture\n");
        logo_box.w = Screen::WIDTH / 2;
        logo_box.h = Screen::HEIGHT / 8;
    }
    logo_box.x = Screen::WIDTH / 2 - logo_box.w / 2;
    logo_box.y = Screen::HEIGHT / 16;

    const int NUM_LINES = 5;
    SDL_Rect text_box;
    text_box.w = Screen::WIDTH / 2;
    text_box.h = Screen::HEIGHT / 16;
    text_box.x = Screen::WIDTH / 2 - text_box.w / 2;
    const int first_y = Screen::HEIGHT / 2 - (text_box.h * NUM_LINES) / 2;
    // text_box.y is set in the loop (starting with first_y)

    int padding = text_box.h / 2;

    // create text textures
    SDL_Texture* title_text[NUM_LINES];
    title_text[0] = s.create_text_texture(
            "Quick Play     ", NULL);
    title_text[1] = s.create_text_texture(
            "How to Play    ", NULL);
    title_text[2] = s.create_text_texture(
            "Versus Mode    ", NULL);
    title_text[3] = s.create_text_texture(
            "Watch the Pilot", NULL);
    title_text[4] = s.create_text_texture(
            "Quit           ", NULL);

    int selection = 0;
    MenuOption options[] = {QUICK_PLAY, HOW_TO_PLAY, VERSUS, WATCH_PILOT, EXIT};
    SDL_Rect selection_box;
    selection_box.x = text_box.x - 10;
    selection_box.w = text_box.w + 20;
    selection_box.h = text_box.h + padding;
    // selection_box.y is set in the loop

    while (true) {
        // write text to the screen
        s.clear();
        SDL_RenderCopy(s.renderer, logo, NULL, &logo_box);
        text_box.y = first_y;
        for (int i = 0; i < NUM_LINES; i++) {
            SDL_RenderCopy(s.renderer, title_text[i], NULL, &text_box);
            text_box.y += text_box.h + padding;
        }

        // draw a box around selected option
        // save old color
        Uint8 old_r, old_g, old_b, old_a;
        SDL_GetRenderDrawColor(s.renderer, &old_r, &old_g, &old_b, &old_a);
        SDL_SetRenderDrawColor(s.renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        selection_box.y = selection * selection_box.h + 
                          first_y - padding / 2;
        SDL_RenderDrawRect(s.renderer, &selection_box);

        // reset old color
        SDL_SetRenderDrawColor(s.renderer, old_r, old_g, old_b, old_a);

        s.flip();

        // Read input
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                return EXIT;
           } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE ||
                    e.key.keysym.sym == SDLK_q) {
                    return EXIT;
                } else if (e.key.keysym.sym == SDLK_SPACE ||
                           e.key.keysym.sym == SDLK_RETURN ||
                           e.key.keysym.sym == SDLK_KP_ENTER) {
                    return options[selection];
                } else if (e.key.keysym.sym == SDLK_UP) {
                    selection--;
                    if (selection < 0) {
                        // wraparound
                        selection = NUM_LINES - 1;
                    }
                } else if (e.key.keysym.sym == SDLK_DOWN) {
                    selection++;
                    if (selection == NUM_LINES) {
                        // wraparound
                        selection = 0;
                    }
                }
            }
        }
        SDL_Delay(50);
    }
    
}

// How to play and press any key to begin.
// returning true means play and false means quit
//
// if playing_next is true, this screen also serves as
// a start screen
Menu::QuitType Menu::how_to_play(Screen& s, bool playing_next) {
    // set up rectangle for text
    SDL_Rect where;
    where.w = 3 * Screen::WIDTH / 4;
    where.h = Screen::HEIGHT / 16;
    where.x = Screen::WIDTH / 2 - where.w / 2;
    const int NUM_LINES = 7;
    int first_y = Screen::HEIGHT / 2 - where.h * (NUM_LINES + 1) / 2;
    // where.y is set in the loop

    // create text textures
    SDL_Texture* text[NUM_LINES];
    text[0] = s.create_text_texture(
            "               HOW TO PLAY                  ", NULL);
    text[1] = s.create_text_texture(
            "left  or a     apply torque counterclockwise", NULL);
    text[2] = s.create_text_texture(
            "right or d     apply torque clockwise       ", NULL);
    text[3] = s.create_text_texture(
            "Spacebar       fire thruster                ", NULL);
    text[4] = s.create_text_texture(
            "up    or w     increase thrust              ", NULL);
    text[5] = s.create_text_texture(
            "down  or s     decrease thrust              ", NULL);

    if (playing_next) {
        text[6] = s.create_text_texture(
            "            Press any key to start          ", NULL);
    } else {
        text[6] = s.create_text_texture(
            "      Press any key to return to menu       ", NULL);
    }


    SDL_Event e;
    while (true) {
        // write text to the screen
        s.clear();
        where.y = first_y;
        for (int i = 0; i < NUM_LINES; i++) {
            SDL_RenderCopy(s.renderer, text[i], NULL, &where);
            where.y += where.h;
            if (i == NUM_LINES - 2) {
                // skip a line before "press any key"
                where.y += where.h;
            }
        }
        s.flip();

        // Read input
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                return END_PROGRAM;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    return TO_MENU;
                // filter out keys that shouldn't count
                // as a keypress, like control
                } else if (!Utils::is_mod_key(e.key.keysym.sym)) {
                    return NO_QUIT;
                }
            }
        }
        SDL_Delay(100);
    }
}

Menu::QuitType Menu::quick_play(Screen& s, Pilot* pilot, unsigned int seed) {
    // initialize, then call play() in a loop
    QuitType quit_type = NO_QUIT;
    if (pilot == NULL) {
        quit_type = how_to_play(s, true);
    }
    Play::PlayResult result(false, Play::EndGameOpt::QUIT, 0); // dummy values

    while (quit_type == NO_QUIT) {
        // play the game
        result = Play::play(&s, pilot, seed);

        // read their choice (Quit, return to menu, New game, or replay)
        if (result.choice == Play::EndGameOpt::QUIT) {
            quit_type = QuitType::END_PROGRAM;
        } else if (result.choice == Play::EndGameOpt::RETURN_TO_MENU) {
            quit_type = QuitType::TO_MENU;
        } else if (result.choice == Play::EndGameOpt::NEW_GAME) {
            seed = (unsigned int) time(NULL);
        }

        // recreate the Pilot if we need to
        if (pilot != NULL) {
            delete pilot;
            if (quit_type == QuitType::NO_QUIT) {
                pilot = new Pilot();
            }
        }
    }

    return quit_type;
}

