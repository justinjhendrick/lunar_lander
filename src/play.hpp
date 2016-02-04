#ifndef PLAY_H_
#define PLAY_H_

#include "../src/Screen.hpp"
#include "../src/Pilot.hpp"

enum EndGameOpt {
    NEW_GAME,
    REPLAY,
    QUIT
};

class PlayResult {
    public:
        PlayResult(bool _win, EndGameOpt _choice);
        bool win;
        EndGameOpt choice;
};

PlayResult play(Screen* s, Pilot* pilot, unsigned int seed);

#endif
