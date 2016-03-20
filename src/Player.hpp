#ifndef PLAYER_H_
#define PLAYER_H_

#include "Pilot.hpp"
#include "Colors.hpp"

class Player {
    public:
        Player();
        ~Player();
        bool is_human();

        // call this function between games
        void prep_for_next_game();

    private:
        Pilot pilot; // NULL means human
        Colors::PlayerColor color;
        unsigned int score;
};

#endif
