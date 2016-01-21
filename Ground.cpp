#include "Ground.hpp"
Ground::Ground(bool _is_pad, int x1, int y1, int x2, int y2) :
    begin((float) x1, (float) y1),
    segment((float) x2 - x1, (float) y2 - y1) {
    is_pad = _is_pad;
}

void Ground::draw(Screen& s) {
    if (is_pad) {
        // save old color
        Uint8 old_r, old_g, old_b, old_a;
        SDL_GetRenderDrawColor(s.renderer, &old_r, &old_g, &old_b, &old_a);

        // 398ee7
        SDL_SetRenderDrawColor(s.renderer, 0x39, 0x8E, 0xE7, 0xFF);
        Vector p2 = Vector::plus(begin, segment);
        SDL_Rect r;
        r.x = begin.x;
        r.y = begin.y;
        r.w = segment.x;
        r.h = 10;
        SDL_RenderFillRect(s.renderer, &r);

        // reset to old color
        SDL_SetRenderDrawColor(s.renderer, old_r, old_g, old_b, old_a);
    }
}

int Ground::get_right() {
    return begin.x + segment.x;
}

int Ground::get_center() {
    return begin.x + segment.x / 2;
}
