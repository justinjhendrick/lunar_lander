#include <SDL2/SDL.h>
#include <cmath>
#include "Lander.hpp"
#include "Screen.hpp"

Lander::Lander(Screen& s) :
    Lander(s,
           Screen::WIDTH / 2,  // x_pos
           Screen::HEIGHT / 2,  // y_pos
           0.,  // x_vel
           0.,  // y_vel
           0.,  // orientation
           0.,  // spin_rate
           .1,  // max_torque
           1.,  // fuel
           1.,  // dry_mass
           1.,  // init_fuel
           0.,  // thrust
           10. // max_thrust
    ) {
}

Lander::Lander(Screen& s,
       float _x_pos,
       float _y_pos,
       float _x_vel,
       float _y_vel,
       float _orientation,
       float _spin_rate,
       float _max_torque,
       float _fuel,
       float _dry_mass,
       float _init_fuel,
       float _thrust,
       float _max_thrust) {
    // set init vals
    x_pos = _x_pos;
    y_pos = _y_pos;
    x_vel = _x_vel;
    y_vel = _y_vel;
    orientation = _orientation;
    spin_rate = _spin_rate;
    max_torque = _max_torque;
    fuel = _fuel;
    dry_mass = _dry_mass;
    init_fuel = _init_fuel;
    thrust = _thrust;
    max_thrust = _max_thrust;

    // load texture(s)
    SDL_Surface* surf = SDL_LoadBMP("sprites/lander_no_alpha.bmp");
    if (surf == NULL) {
        fprintf(stderr, "no load lander: %s\n", SDL_GetError());
    }
    txtr = SDL_CreateTextureFromSurface(s.renderer, surf);
    if (txtr == NULL) {
        fprintf(stderr, "no convert lander to texture: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(surf);
}

void Lander::handle(SDL_Event* e) {
    if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
            case SDLK_UP:
                thrust += max_thrust / 10;
                if (thrust > max_thrust) {
                    thrust = max_thrust;
                }
                break;
            case SDLK_DOWN:
                thrust -= max_thrust / 10;
                if (thrust < 0) {
                    thrust = 0;
                }
                break;
            case SDLK_LEFT:
                torque = -max_torque;
                break;
            case SDLK_RIGHT:
                torque = max_torque;
                break;
            case SDLK_SPACE:
                if (fuel > 0.) {
                    thrusting = true;
                }
                break;
            default:
                break;
        }
    } else if (e->type == SDL_KEYUP) {
        switch (e->key.keysym.sym) {
            case SDLK_LEFT:
                torque = 0;
                break;
            case SDLK_RIGHT:
                torque = 0;
                break;
            case SDLK_SPACE:
                thrusting = false;
                break;
            default:
                break;
        }
    }
}

void Lander::move() {
    spin_rate += torque;
    orientation += spin_rate;

    if (thrusting) {
        float x_accel = thrust * cos(orientation) / (dry_mass + fuel);
        float y_accel = thrust * sin(orientation) / (dry_mass + fuel);
        x_vel += x_accel;
        y_vel += y_accel;
    }

    x_pos += x_vel;
    y_pos += y_vel;
}

void Lander::draw(Screen& s) {
    SDL_Rect dest;
    dest.x = x_pos;
    dest.y = y_pos;
    dest.w = WIDTH;
    dest.h = HEIGHT;

    //SDL_RenderCopy(s.renderer, txtr, NULL, &dest);
    //SDL_Point rotate_about;
    //rotate_about.x = x_pos + WIDTH / 2;
    //rotate_about.y = y_pos + HEIGHT / 2;
    SDL_RenderCopyEx(s.renderer,
                     txtr,
                     NULL,
                     &dest,
                     (M_PI_2 + orientation) * 180. / M_PI,
                     NULL,
                     SDL_FLIP_NONE
    );
}
