#include <SDL2/SDL.h>
#include "Lander.hpp"
#include "Screen.hpp"

Lander::Lander() :
    Lander(0.,
           0.,
           0.,
           0.,
           0.,
           0.,
           1.,
           10.,
           1.,
           1.,
           0.,
           100.
    ) {
}

Lander::Lander(float _x_pos,
       float _y_pos,
       float _x_vel,
       float _y_vel,
       float _orientation,
       float _spin_rate,
       float _torque,
       float _fuel,
       float _dry_mass,
       float _init_fuel,
       float _thrust,
       float _max_thrust) {
    x_pos = _x_pos;
    y_pos = _y_pos;
    x_vel = _x_vel;
    y_vel = _y_vel;
    orientation = _orientation;
    spin_rate = _spin_rate;
    torque = _torque;
    fuel = _fuel;
    dry_mass = _dry_mass;
    init_fuel = _init_fuel;
    thrust = _thrust;
    max_thrust = _max_thrust;
}

void Lander::draw(Screen& s) {
    // TODO
    Uint8 old_r;
    Uint8 old_g;
    Uint8 old_b;
    Uint8 old_a;
    SDL_GetRenderDrawColor(s.renderer, &old_r, &old_g, &old_b, &old_a);
    SDL_SetRenderDrawColor(s.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_Rect rect;
    rect.x = (int) x_pos;
    rect.y = (int) y_pos;
    rect.w = 100;
    rect.h = 100;
    SDL_RenderFillRect(s.renderer, &rect);
    SDL_SetRenderDrawColor(s.renderer, old_r, old_g, old_b, old_a);
}
