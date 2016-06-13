#include "VersusMode.hpp"


std::vector<Player> VersusMode::player_select(Screen& screen) {
    // TODO: (generalize menu code?)
    std::vector<Player> result;
    return result;
}

Menu::QuitType VersusMode::versus(Screen& screen, unsigned int seed) {
    std::vector<Player> players = player_select(screen);
    
    for (unsigned char round = 0; round < N_ROUNDS; round++) {
        for (Player& p : players) {
            // TODO: make Play::play take a Player
            // refactor all the code t use players anywhere a pilot is used
        }
    }
    return Menu::TO_MENU;
}
