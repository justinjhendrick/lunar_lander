#include <SDL2/SDL.h>
#include <cmath>
#include "Lander.hpp"
#include "Screen.hpp"
#include "constants.hpp"

Lander::Lander(Screen& s) :
    Lander(s,
           Screen::WIDTH / 2,  // x_pos
           Screen::HEIGHT / 2,  // y_pos
           0.,  // x_vel
           0.,  // y_vel
           0.,  // orientation
           0.,  // spin_rate
           .08,  // max_torque
           1.,  // dry_mass
           1.,  // init_fuel
           1.,  // thrust
           1000. // max_thrust
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
    fuel = _init_fuel;
    dry_mass = _dry_mass;
    init_fuel = _init_fuel;
    thrust = _thrust;
    max_thrust = _max_thrust;

    txtr = load_texture(s.renderer, "sprites/lander.bmp");
    txtr_fire_low = load_texture(s.renderer, "sprites/lander_fire_low.bmp");
    txtr_fire_med = load_texture(s.renderer, "sprites/lander_fire_med.bmp");
    txtr_fire_high = load_texture(s.renderer, "sprites/lander_fire_high.bmp");
    dt = FRAME_TIME / 1000.;
}

SDL_Texture* Lander::load_texture(SDL_Renderer* r, const char filename[]) {
    SDL_Surface* surf = SDL_LoadBMP(filename);
    if (surf == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
    }
    SDL_Texture* result = SDL_CreateTextureFromSurface(r, surf);
    if (txtr == NULL) {
        fprintf(stderr, "no convert to texture: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(surf);
    return result;
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
        // compute acceleration
        float x_accel = thrust * cos(orientation) / (dry_mass + fuel);
        float y_accel = thrust * sin(orientation) / (dry_mass + fuel);

        float new_x_vel = x_vel + x_accel * dt;
        float new_y_vel = y_vel + y_accel * dt;
        
        // calculate mass change
        //float vel = sqrt(x_vel * x_vel + y_vel * y_vel);
        //float new_vel = sqrt(new_x_vel * new_x_vel + new_y_vel * new_y_vel);
        //printf("v = %f -> %f\n", vel, new_vel);
        //printf("%f / %f = ", dt * thrust + (dry_mass + fuel) * vel, new_vel);
        //float new_mass = (dt * thrust + (dry_mass + fuel) * vel) /
        //                 new_vel;
        //printf("%f\n", new_mass);
        //fuel -= (dry_mass + fuel) - new_mass;
        float exhaust_vel = 100.;
        float dmdt = ((dry_mass + fuel) * sqrt(pow(new_x_vel - x_vel, 2) +
                     pow(new_y_vel - y_vel, 2)) - thrust) / exhaust_vel;
        printf("dmdt = %f\n", dmdt);
        fuel += dmdt * dt;
        printf("%f\n", fuel);

        // apply acceleration
        x_vel = new_x_vel;
        y_vel = new_y_vel;
    }
    //y_vel += .1; // gravity

    x_pos += x_vel * dt;
    y_pos += y_vel * dt;
}

void Lander::draw(Screen& s) {
    SDL_Rect dest;
    dest.x = (int) x_pos;
    dest.y = (int) y_pos;
    dest.w = WIDTH;
    dest.h = HEIGHT;

    SDL_Texture* t = txtr;
    if (thrusting) {
        if (thrust < max_thrust / 3) {
            t = txtr_fire_low;
        } else if (thrust < 2 * max_thrust / 3) {
            t = txtr_fire_med;
        } else {
            t = txtr_fire_high;
        }
    }

    //SDL_RenderCopy(s.renderer, txtr, NULL, &dest);
    //SDL_Point rotate_about;
    //rotate_about.x = x_pos + WIDTH / 2;
    //rotate_about.y = y_pos + HEIGHT / 2;
    SDL_RenderCopyEx(s.renderer,
                     t,
                     NULL,
                     &dest,
                     (M_PI_2 + orientation) * 180. / M_PI,
                     NULL,
                     SDL_FLIP_NONE
    );
}

Lander::~Lander() {
    SDL_DestroyTexture(txtr);
    SDL_DestroyTexture(txtr_fire_low);
    SDL_DestroyTexture(txtr_fire_med);
    SDL_DestroyTexture(txtr_fire_high);
}
