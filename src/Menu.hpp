#ifndef MENU_H_
#define MENU_H_

#include "Screen.hpp"
#include "Pilot.hpp"

class Menu {
    public:
        enum MenuOption {
            QUICK_PLAY,
            HOW_TO_PLAY,
            VERSUS,
            WATCH_PILOT,
            EXIT
        };

        enum QuitType {
            NO_QUIT, // don't quit
            TO_MENU,
            END_PROGRAM
        };

        static MenuOption menu(Screen& s);

        static QuitType how_to_play(Screen& s, bool playing_next);

        // return value of false means kill the program (X button was clicked)
        static QuitType quick_play(Screen& s, Pilot* pilot, unsigned int seed);
};

#endif
