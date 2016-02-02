#include "Screen.hpp"

Screen::Screen() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL no init: %s\n", SDL_GetError());
    }
    if (SDL_CreateWindowAndRenderer(WIDTH,
                                    HEIGHT,
                                    SDL_WINDOW_SHOWN,
                                    &window,
                                    &renderer) < 0) {
        fprintf(stderr, "No window & renderer: %s\n", SDL_GetError());
    }
    SDL_SetWindowTitle(window, "Lunar Lander");
    if (TTF_Init() == -1) {
        fprintf(stderr, "no ttf: %s\n", TTF_GetError());
    }
    font = TTF_OpenFont("UbuntuMono-R.ttf", 12);
    if (font == NULL) {
        fprintf(stderr, "no font: %s\n", TTF_GetError());
    }

    win_text = create_text_texture("You win!", NULL);
    lose_text = create_text_texture("You lose.", NULL);
    newgame_text = create_text_texture(
            "press n to play a new level     ", NULL);
    replay_text = create_text_texture(
            "press r to replay the same level", NULL);
    quit_text = create_text_texture(
            "press q to quit                 ", NULL);
}

void Screen::put_endgame_text(bool win) {
    SDL_Rect where;
    where.w = 3 * Screen::WIDTH / 4;
    where.h = Screen::HEIGHT / 8;
    where.x = Screen::WIDTH / 2 - where.w / 2;
    where.y = Screen::HEIGHT / 4 - where.h / 2;

    SDL_Texture* win_or_lose = win ? win_text : lose_text;
    SDL_RenderCopy(renderer, win_or_lose, NULL, &where);

    where.y += Screen::HEIGHT / 8;
    SDL_RenderCopy(renderer, newgame_text, NULL, &where);

    where.y += Screen::HEIGHT / 8;
    SDL_RenderCopy(renderer, replay_text, NULL, &where);

    where.y += Screen::HEIGHT / 8;
    SDL_RenderCopy(renderer, quit_text, NULL, &where);
}

Screen::~Screen() {
    SDL_DestroyTexture(win_text);
    SDL_DestroyTexture(lose_text);
    SDL_DestroyTexture(newgame_text);
    SDL_DestroyTexture(replay_text);
    SDL_DestroyTexture(quit_text);

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
}

void Screen::flip() {
    SDL_RenderPresent(renderer);
}

void Screen::clear() {
    SDL_RenderClear(renderer);
}

SDL_Texture* Screen::load_texture(const char filename[]) {
    SDL_Surface* surf = SDL_LoadBMP(filename);
    if (surf == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return NULL;
    }
    SDL_Texture* result = SDL_CreateTextureFromSurface(renderer, surf);
    if (result == NULL) {
        fprintf(stderr, "no convert to texture: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(surf);
    return result;
}

SDL_Texture* Screen::create_text_texture(const char text[],
                                         SDL_Color* color) {
    SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
    if (color == NULL) {
        color = &white;
    }
    SDL_Surface* surf = TTF_RenderText_Solid(font, text, *color);
    if (surf == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return NULL;
    }
    SDL_Texture* result = SDL_CreateTextureFromSurface(renderer, surf);
    if (result == NULL) {
        fprintf(stderr, "no convert to texture: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(surf);
    return result;
}
