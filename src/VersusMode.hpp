#ifndef VERSUSMODE_H_
#define VERSUSMODE_H_

#include "Menu.hpp"
#include "Player.hpp"

class VersusMode {
    private:
        std::vector<Player> versus_menu(Screen& screen);
    public:
        Menu::QuitType versus(Screen& screen, unsigned int seed);
};

#endif
