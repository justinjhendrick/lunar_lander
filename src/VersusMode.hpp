#ifndef VERSUSMODE_H_
#define VERSUSMODE_H_

#include "Menu.hpp"
#include "Player.hpp"

class VersusMode {
    private:
        static std::vector<Player> player_select(Screen& screen);
    public:
        static const unsigned char N_ROUNDS = 3;
        static Menu::QuitType versus(Screen& screen, unsigned int seed);
};

#endif
