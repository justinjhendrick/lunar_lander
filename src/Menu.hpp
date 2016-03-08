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

        static MenuOption menu(Screen& s);

        static bool how_to_play(Screen& s);
        static void quick_play(Screen& s, Pilot* pilot, unsigned int seed);
};

#endif
