#include "Ground.hpp"
Ground::Ground(bool _is_pad, int x1, int y1, int x2, int y2) :
    begin((float) x1, (float) y1),
    segment((float) x2 - x1, (float) y2 - y1) {
    is_pad = _is_pad;
}

bool Ground::is_colliding(const Lander& l) {
    Vector p1(l.p1x, l.p1y);
    Vector p2(l.p2x, l.p2y);
    Vector p3(l.p3x, l.p3y);

    Vector p1_to_p2 = Vector::minus(p2, p1);
    Vector p2_to_p3 = Vector::minus(p3, p2);
    Vector p3_to_p1 = Vector::minus(p1, p3);

    bool left_collide = Vector::segments_intersect(p1, p1_to_p2, begin, segment);
    bool right_collide = Vector::segments_intersect(p2, p2_to_p3, begin, segment);
    bool bot_collide = Vector::segments_intersect(p3, p3_to_p1, begin, segment);
    if (left_collide || right_collide || bot_collide) {
        return true;
    }
    return false;
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
