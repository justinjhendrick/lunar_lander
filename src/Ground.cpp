#include <assert.h>
#include "Ground.hpp"
#include "Colors.hpp"

Ground::Ground(bool _is_pad, int x1, int y1, int x2, int y2) :
    Line(x1, y1, x2, y2) {
    is_pad = _is_pad;
}

void Ground::draw(Screen& s) {
    if (is_pad) {
        // save old color
        Uint8 old_r, old_g, old_b, old_a;
        SDL_GetRenderDrawColor(s.renderer, &old_r, &old_g, &old_b, &old_a);

        // 398ee7 is blue
        SDL_SetRenderDrawColor(s.renderer,
                               Colors::PAD.r,
                               Colors::PAD.g,
                               Colors::PAD.b,
                               Colors::PAD.a);
        SDL_Rect r;
        r.x = (int) begin.x;
        r.y = (int) begin.y;
        r.w = (int) segment.x;
        r.h = PAD_HEIGHT;
        SDL_RenderFillRect(s.renderer, &r);

        // reset to old color
        SDL_SetRenderDrawColor(s.renderer, old_r, old_g, old_b, old_a);
    } else {
        // save old color
        Uint8 old_r, old_g, old_b, old_a;
        SDL_GetRenderDrawColor(s.renderer, &old_r, &old_g, &old_b, &old_a);

        SDL_SetRenderDrawColor(s.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderDrawLine(s.renderer,
                           (int) begin.x,
                           (int) begin.y,
                           (int) (begin.x + segment.x),
                           (int) (begin.y + segment.y)
        );

        // reset to old color
        SDL_SetRenderDrawColor(s.renderer, old_r, old_g, old_b, old_a);
    }
}
