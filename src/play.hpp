#ifndef PLAY_H_
#define PLAY_H_

#include "Screen.hpp"
#include "Pilot.hpp"

enum EndGameOpt {
    NEW_GAME,
    REPLAY,
    QUIT
};

class PlayResult {
    public:
        PlayResult(bool _win, EndGameOpt _choice, unsigned int _score);
        bool win;
        EndGameOpt choice;
        unsigned int score;
};

PlayResult play(Screen* s, Pilot* pilot, unsigned int seed);

#endif
