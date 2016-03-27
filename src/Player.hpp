#ifndef PLAYER_H_
#define PLAYER_H_

#include "Pilot.hpp"
#include "Colors.hpp"

// TODO:
// move event handling code into here (maybe).
// With a function that calls Pilot::fly() for the pilot
// and handles events for a human
class Player {
    public:
        Player(bool human, Colors::PlayerColor _color);
        ~Player();
        bool is_human();

        // call this function between rounds
        // (score persists across this call)
        void prep_for_next_round();

    private:
        Pilot* pilot; // NULL means human
        Colors::PlayerColor color;
        unsigned int score;
};

#endif
