#ifndef PLAY_H_
#define PLAY_H_

#include "Screen.hpp"
#include "Pilot.hpp"

class Play {
    public:
        enum EndGameOpt {
            NEW_GAME,
            REPLAY,
            RETURN_TO_MENU,
            QUIT
        };

        class PlayResult {
            public:
                PlayResult(bool _win, EndGameOpt _choice, unsigned int _score);
                bool win;
                EndGameOpt choice;
                unsigned int score;
        };

        // play the game defined by the seed.
        // A NULL pilot is a human player
        // a NULL screen means headless mode. (used for testing)
        static PlayResult play(Screen* s, Pilot* pilot, unsigned int seed);

    private:
        static EndGameOpt end_game(Screen& s,
                                   World& world,
                                   Lander& lander,
                                   bool win);

        // x must be > 0
        // scale * 2 ^ (-x / stretch)
        static unsigned int exponential_decay(double scale,
                                              double stretch,
                                              double x);
        static unsigned int compute_score(bool win,
                                          unsigned long frames,
                                          double fuel_percent);
};

#endif
