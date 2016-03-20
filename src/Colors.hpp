#ifndef COLORS_H_
#define COLORS_H_

#include <SDL2/SDL.h>

class Colors {
    public:
        enum PlayerColor {
            WHITE,
            GREEN,
            ORANGE
        };
        const static constexpr SDL_Color PLAYER_WHITE  = {0xFF, 0xFF, 0xFF, 0xFF};
        const static constexpr SDL_Color PLAYER_GREEN  = {0x2F, 0x9B, 0x14, 0xFF};
        const static constexpr SDL_Color PLAYER_ORANGE = {0xEF, 0x6F, 0x41, 0xFF};
        const static constexpr SDL_Color PAD = {0x39, 0x8E, 0xE7, 0xFF};
};

#endif
