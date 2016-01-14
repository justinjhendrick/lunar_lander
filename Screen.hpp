#ifndef SCREEN_H_
#define SCREEN_H_

#include <SDL2/SDL.h>

class Screen {
    private:
        static const int WIDTH = 640;
        static const int HEIGHT = 480;
    public:
        SDL_Window* window;
        SDL_Renderer* renderer;

        Screen();
        ~Screen();
        void flip();
        void clear();
};

#endif
