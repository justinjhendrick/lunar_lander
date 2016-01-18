#include "Ground.hpp"
Ground::Ground(bool _is_pad, int x1, int y1, int x2, int y2) :
    begin((float) x1, (float) y1),
    segment((float) x2 - x1, (float) y2 - y1) {
    is_pad = _is_pad;
}

void Ground::draw(Screen& s) {
    // save old color
    Uint8 old_r, old_g, old_b, old_a;
    SDL_GetRenderDrawColor(s.renderer, &old_r, &old_g, &old_b, &old_a);

    SDL_SetRenderDrawColor(s.renderer, 0x55, 0x55, 0xFF, 0xFF);
    Vector p2 = Vector::plus(begin, segment);
    SDL_RenderDrawLine(s.renderer, begin.x, begin.y, p2.x, p2.y);

    // reset to old color
    SDL_SetRenderDrawColor(s.renderer, old_r, old_g, old_b, old_a);
}
