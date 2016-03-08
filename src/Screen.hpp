#ifndef SCREEN_H_
#define SCREEN_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// A class that represents the screen
// using SDL rendering (which is capable of hardware acceleration).
// https://wiki.libsdl.org/CategoryRender
class Screen {
    private:
        SDL_Window* window;

        // keep some textures that we reuse here
        SDL_Texture* win_text;
        SDL_Texture* lose_text;
        SDL_Texture* newgame_text;
        SDL_Texture* replay_text;
        SDL_Texture* quit_text;
    public:
        static const int WIDTH = 640;
        static const int HEIGHT = 480;
        SDL_Renderer* renderer;
        TTF_Font* font;

        Screen();
        ~Screen();

        void put_endgame_text(bool win);

        // put the buffered pixels to the screen
        void flip();
        // set all pixels on screen to black
        void clear();
        // use clear() and flip() like this
        // while () {
        //     clear()
        //     draw()
        //     flip()
        //     sleep()
        // }
        
        // load a BMP
        SDL_Texture* load_texture(const char filename[]);

        // create a Texture with text in it
        // if color is NULL, white is used.
        SDL_Texture* create_text_texture(const char text[],
                                         SDL_Color* color);

        // print to stderr on failure
        static SDL_Surface* load_bmp(const char filename[]);
        SDL_Texture* surf_to_txtr(SDL_Surface* s);
};

#endif
