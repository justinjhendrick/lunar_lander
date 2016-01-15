#include <SDL2/SDL.h>
#include <cmath>
#include "Lander.hpp"
#include "Screen.hpp"
#include "constants.hpp"
#include "Utils.hpp"

Lander::Lander(Screen& s) :
    Lander(s,
           Screen::WIDTH / 2,  // x_pos
           Screen::HEIGHT / 2, // y_pos
           0.,     // x_vel
           0.,     // y_vel
           0.,     // orientation
           0.,     // spin_rate
           .08,    // max_torque
           2150.,  // dry_mass
           2353.,  // init_fuel
           0.,     // thrust
           16000., // max_thrust
           3050.   // exhaust velocity
    ) {
    // values above from the Apollo lunar module
    // https://en.wikipedia.org/wiki/Apollo_Lunar_Module
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
       float _max_thrust,
       float _exhaust_vel) {
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
    exhaust_vel = _exhaust_vel;

    torque = 0.;
    dt = FRAME_TIME / 1000.;
    txtr = Utils::load_texture(s.renderer, "sprites/lander.bmp");
    txtr_fire_low = Utils::load_texture(s.renderer, "sprites/lander_fire_low.bmp");
    txtr_fire_med = Utils::load_texture(s.renderer, "sprites/lander_fire_med.bmp");
    txtr_fire_high = Utils::load_texture(s.renderer, "sprites/lander_fire_high.bmp");
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
                if (fuel > 0. && thrust > 0.) {
                    thrusting = true;
                }
                break;
            default:
                break;
        }
    } else if (e->type == SDL_KEYUP) {
        switch (e->key.keysym.sym) {
            case SDLK_LEFT:
                torque = 0.;
                break;
            case SDLK_RIGHT:
                torque = 0.;
                break;
            case SDLK_SPACE:
                thrusting = false;
                break;
            default:
                break;
        }
    }
}

void Lander::craft_to_sdl_coords() {
    float s = sin(3 * M_PI_2 + orientation);
    float c = cos(3 * M_PI_2 + orientation);
    p1x = c * sc_p1x - s * sc_p1y + WIDTH / 2 + x_pos;
    p1y = s * sc_p1x + c * sc_p1y + COLLISION_HEIGHT + y_pos;

    p2x = c * sc_p2x - s * sc_p2y + WIDTH / 2 + x_pos;
    p2y = s * sc_p2x + c * sc_p2y + COLLISION_HEIGHT + y_pos;

    p3x = c * sc_p3x - s * sc_p3y + WIDTH / 2 + x_pos;
    p3y = s * sc_p3x + c * sc_p3y + COLLISION_HEIGHT + y_pos;
}

void Lander::move() {
    spin_rate += torque;
    orientation += spin_rate;
    if (orientation > 2 * M_PI) {
        orientation -= 2 * M_PI;
    } else if (orientation < 0) {
        orientation += 2 * M_PI;
    }

    float x_accel = 0.;
    float y_accel = 0.;
    if (thrusting) {
        // compute acceleration
        x_accel = thrust * cos(orientation) / (dry_mass + fuel);
        y_accel += thrust * sin(orientation) / (dry_mass + fuel);

        float new_x_vel = x_vel + x_accel * dt;
        float new_y_vel = y_vel + y_accel * dt;
        
        // calculate mass change
        float dmdt = ((dry_mass + fuel) * sqrt(pow(new_x_vel - x_vel, 2) +
                     pow(new_y_vel - y_vel, 2) / dt) - thrust) / exhaust_vel;
        fuel += dmdt * dt;
    }
    x_vel += x_accel;
    y_vel += y_accel;// + 1.62; // gravity

    // calculate new position
    x_pos += x_vel * dt + .5 * x_accel * dt * dt;
    y_pos += y_vel * dt + .5 * y_accel * dt * dt;

    // recalculate collision points
    craft_to_sdl_coords();
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

    SDL_Point rot_abt;
    rot_abt.x = WIDTH / 2;
    rot_abt.y = COLLISION_HEIGHT;
    SDL_RenderCopyEx(s.renderer,
                     t,
                     NULL,
                     &dest,
                     (M_PI_2 + orientation) * 180. / M_PI,
                     &rot_abt,
                     SDL_FLIP_NONE
    );

    // rectangle for debugging
    Uint8 old_r, old_g, old_b, old_a;
    SDL_GetRenderDrawColor(s.renderer, &old_r, &old_g, &old_b, &old_a);
    SDL_SetRenderDrawColor(s.renderer, 0xff, 0x00, 0x00, 0xFF);
    SDL_Rect r;
    r.x = (int) p1x;
    r.y = (int) p1y;
    r.w = 2;
    r.h = 2;
    SDL_RenderDrawRect(s.renderer, &r);

    r.x = (int) p2x;
    r.y = (int) p2y;
    SDL_RenderDrawRect(s.renderer, &r);

    r.x = (int) p3x;
    r.y = (int) p3y;
    SDL_RenderDrawRect(s.renderer, &r);
    SDL_SetRenderDrawColor(s.renderer, old_r, old_g, old_b, old_a);

}

Lander::~Lander() {
    SDL_DestroyTexture(txtr);
    SDL_DestroyTexture(txtr_fire_low);
    SDL_DestroyTexture(txtr_fire_med);
    SDL_DestroyTexture(txtr_fire_high);
}
